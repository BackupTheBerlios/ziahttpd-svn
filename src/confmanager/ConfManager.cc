//
// ConfLoader.cpp for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Sat Oct 22 10:25:16 2005 Bigand Xavier
// Last update Tue Nov  8 12:31:41 2005 
//

#include <ctype.h>
#include "ConfManager.hh"



//
// Private members
//



void	ConfManager::init_fct_ptr()
{
  // Faire attention a ce que NB_CONTAINER soit correctement definie

  _Container[0].sContainer = "requiere";
  _Container[0].fct = &ConfManager::ManageRequiere;
  _Container[1].sContainer = "include";
  _Container[1].fct = &ConfManager::ManageInclude;
  _Container[2].sContainer = "var";
  _Container[2].fct = &ConfManager::ManageVar;
  _Container[3].sContainer = "del";
  _Container[3].fct = &ConfManager::ManageDel;
  _Container[4].sContainer = "list";
  _Container[4].fct = &ConfManager::ManageList;
  _Container[5].sContainer = "eval";
  _Container[5].fct = &ConfManager::ManageEval;
}

int	ConfManager::Load(string sConfFile)
{
  TiXmlDocument	*_pConfFile;

  _svListInclude.push_back(sConfFile);
  _pConfFile = new TiXmlDocument(sConfFile.c_str());
  if (_pConfFile->LoadFile())
    {
      cout << "Chargement du fichier de configuration." << endl;
      cout << "Traitement du fichier de configuration." << endl << endl;
      DumpToMemory(_pConfFile);
      cout << endl << "Traitement du fichier configuration termine." << endl;
      cout << "Verification des parametres manquants." << endl;
      // verifier que la validite des parametres indispensable
    }
  else
    {
      // voir pour indiquer les erreurs de syntaxe ou autre
      cout << "Erreur lors du chargement du fichier de configration." << endl;
      cout << "Initialisation de la configuration de base." << endl;
    }
  delete _pConfFile;
  return true;
}

void		ConfManager::GetValues(TiXmlNode *pCurrentContainer, string &sValue, tStringVector &svValue)
{
  if (!pCurrentContainer)
    return ;

  if (pCurrentContainer->ToText()) // contient directement une simple valeur
    sValue = (pCurrentContainer->ToText())->ValueStr(); // can copy ""
  else // contient d'autres balises (comme une var deja declaree)
    {
      TiXmlNode	*pChildContainer;
      string	sName;
      string	sElement;	// type of element (var, list, ...)

//       if (pCurrentContainer->ToElement())
// 	{
      sName = MyAttribute(pCurrentContainer->ToElement(), "name");
      sElement = pCurrentContainer->ValueStr();
// 	}
      pChildContainer = pCurrentContainer->FirstChild();
      if (pChildContainer)
	{
	  string	sTmp;
	  tStringVector	svTmp;

	  GetValues(pChildContainer, sTmp, svTmp); // debut de la recursivite
	  if (InsensitiveCmp(sElement, "var") && sTmp != "") // ne pas remplacer une valeur par ""
	    _mSimpleData[sName] = sTmp; // replace old value by new
	  else if (InsensitiveCmp(sElement, "list") && !svTmp.empty())
	    _mListData[sName].insert(_mListData[sName].begin(), svTmp.begin(), svTmp.end()); // add new vector at the old
	}
      sValue = _mSimpleData[sName];
      svValue = _mListData[sName];
    }
}

string	ConfManager::MyAttribute(TiXmlElement *pElement, string sAttribute)
{
  if (!pElement)
    return "";

  const char		*tmp;	// for protect string to NULL return
  string		sCurrentAttribute;
  TiXmlAttribute	*pCurrentAttribute;
  int			iStop;

  transform(sAttribute.begin(), sAttribute.end(), sAttribute.begin(), to_lower_2);
  for (pCurrentAttribute = pElement->FirstAttribute(), iStop = 0;
       !iStop && pCurrentAttribute;
       pCurrentAttribute = pCurrentAttribute->Next())
    {
      iStop = 0;
      tmp = pCurrentAttribute->Name();
      if (tmp)
	{
	  sCurrentAttribute = tmp;
	  transform(sCurrentAttribute.begin(), sCurrentAttribute.end(),
		    sCurrentAttribute.begin(), tolower);
	  if (sCurrentAttribute == sAttribute)
	    {
	      iStop = 1;
	      tmp = pCurrentAttribute->Value();
	      if (tmp)
		return tmp;
	      return "";
	    }
	}
    }
  return "";
}

int	ConfManager::InsensitiveCmp(string sValue1, string sValue2)
{
  transform(sValue1.begin(), sValue1.end(), sValue1.begin(), tolower);
  transform(sValue2.begin(), sValue2.end(), sValue2.begin(), tolower);
  return (sValue1 == sValue2);
}

string	ConfManager::Eval_Expression(TiXmlNode *pCurrentContainer, bool *pbRes)
{
  string	sValue1;
  string	sValue2;
  string	sCompartor;

  sValue1 = MyAttribute(pCurrentContainer->ToElement(), "value1");
  sValue2 = MyAttribute(pCurrentContainer->ToElement(), "value2");
  if (pCurrentContainer->ToText())
    sCompartor = (pCurrentContainer->ToText())->ValueStr();
  if (sCompartor == "equal")
    {
      if (sValue1 == sValue2)
	{
	  *pbRes = true;
	  return EXPR_TRUE;
	}
      *pbRes = false;
      return EXPR_FALSE;
    }
  if (sCompartor == "diff")
    {
      if (sValue1 != sValue2)
	{
	  *pbRes = true;
	  return EXPR_TRUE;
	}
      *pbRes = false;
      return EXPR_FALSE;
    }
  *pbRes = false;
  return EXPR_FALSE;
}



//
// Protected members
//



TiXmlNode	*ConfManager::ManageRequiere(TiXmlNode *pCurrentContainer)
{
  string	value;
  TiXmlNode	*pChildContainer;

  pChildContainer = pCurrentContainer->FirstChild();
  value = (pChildContainer->ToText())->ValueStr();
  return pCurrentContainer->NextSibling();
}

TiXmlNode	*ConfManager::ManageInclude(TiXmlNode *pCurrentContainer)
{
  int				i;
  int				iStop;
  string			sValue;
  TiXmlNode			*pChildContainer;
  tStringVector::iterator	itIterator;

  pChildContainer = pCurrentContainer->FirstChild();
  if (pChildContainer)
    {
      sValue = (pChildContainer->ToText())->ValueStr();
      for (i = 0, iStop = 0, itIterator = _svListInclude.begin();
	   !iStop && itIterator != _svListInclude.end();
	   i++, itIterator++)
	if (sValue == _svListInclude[i])
	  iStop = 1;
      if (!iStop)
	Load(sValue);
    }
  return pCurrentContainer->NextSibling();
}

TiXmlNode	*ConfManager::ManageVar(TiXmlNode *pCurrentContainer)
{
  string			sName;
  string			sValue;
  tStringVector::iterator	itIterator;
  tStringVector			unused;

  GetValues(pCurrentContainer, sValue, unused);
  // This function reconize element var -> auto-set value
  // It's why I give her the current container (element)
  return pCurrentContainer->NextSibling();
}

TiXmlNode	*ConfManager::ManageList(TiXmlNode *pCurrentContainer)
{
  string	sName;
  string	sValue;
  TiXmlNode	*pNextContainer;
  TiXmlNode	*pChildContainer;
  tStringVector	svValue;

  pNextContainer = pCurrentContainer->NextSibling();
  sName = MyAttribute(pCurrentContainer->ToElement(), "name");
  if (sName != "") // do anything if attribute name doesn't exist
    for (pCurrentContainer = pCurrentContainer->FirstChild();
	 pCurrentContainer;
	 pCurrentContainer = pCurrentContainer->NextSibling())
      {
	if (InsensitiveCmp(pCurrentContainer->ValueStr(), "add") &&
	    (pChildContainer = pCurrentContainer->FirstChild()))
	  {
	    GetValues(pChildContainer, sValue, svValue);
	    // This function don't reconize element add -> don't set value
	    // It's why I give her the child container (element)
	    if (sValue != "")
	      _mListData[sName].push_back(sValue);
	    else
	      _mListData[sName].insert(_mListData[sName].begin(), svValue.begin(), svValue.end()); // add new vector at the old
	  }
      }
  return pNextContainer;
}

TiXmlNode	*ConfManager::ManageEval(TiXmlNode *pCurrentContainer, int iFlag, bool *pbRes)
{
  bool		bRes;
  TiXmlNode	*pNextContainer;
  TiXmlNode	*pChildContainer;
  TiXmlNode	*pDoTrueContainer;
  TiXmlNode	*pDoFalseContainer;
  string	sElement;
  string	sAttribute;

  pDoTrueContainer = NULL;
  pDoFalseContainer = NULL;
  pNextContainer = pCurrentContainer->NextSibling();
  for (pCurrentContainer = pCurrentContainer->FirstChild();
       pCurrentContainer;
       pCurrentContainer = pCurrentContainer->NextSibling())
    {
      sElement = pCurrentContainer->ValueStr();
      if (InsensitiveCmp(sElement, "header"))
	{
	  int		i;	// use for test if it must be 'expression' or 'liaison'
	  bool		bTmpRes;
	  bool		bBreak;	// break when the balise order isn't correct
	  int		iOperator;
	  string	sChildElement;
	  string	sChildAttribute;

	  for (pChildContainer = pCurrentContainer->FirstChild(), i = 2,
		 bBreak = false, iOperator = OP_UNDEFINED; pChildContainer && !bBreak;
	       pChildContainer = pChildContainer->NextSibling(), i++)
	    {
	      sChildElement = pChildContainer->ValueStr();
	      if (i % 2 == 0)
		{
		  if (!InsensitiveCmp(sChildElement, "expression"))
		    bBreak = true;
		  else
		    switch (iOperator)
		      {
		      case OP_UNDEFINED:
			Eval_Expression(pChildContainer, &bTmpRes);
			if (i == 2)
			  bRes = bTmpRes;	// init bRes
			break;
		      case OP_OR:
			bRes = bRes || bTmpRes;
			break;
		      case OP_AND:
			bRes = bRes && bTmpRes;
			break;
		      }
		  iOperator = OP_UNDEFINED;	// init operator
		}
	      else
		{
		  if (!InsensitiveCmp(sChildElement, "operator"))
		    bBreak = true;
		  else
		    {
		      sChildAttribute = MyAttribute(pChildContainer->ToElement(), "value");
		      if (sChildAttribute == "or")
			iOperator = OP_OR;
		      else if (sChildAttribute == "and")
			iOperator = OP_AND;
		    }
		}
	    }
	  if (pbRes)
	    *pbRes = bRes;	// for the 'loop' knows when break
	}
      else if (InsensitiveCmp(sElement, "do"))
	{
	  sAttribute = MyAttribute(pCurrentContainer->ToElement(), "type");
	  if (iFlag == EVAL_LOOP || sAttribute == "true")
	    pDoTrueContainer = pCurrentContainer;
	  else if (sAttribute == "false")
	    pDoFalseContainer = pCurrentContainer;
	}
    }
  if ((iFlag == EVAL_LOOP || bRes == true) && pDoTrueContainer)
    DumpToMemory(pDoTrueContainer->FirstChild());
  else if (pDoFalseContainer)
    DumpToMemory(pDoFalseContainer->FirstChild());
  return pNextContainer;
}

TiXmlNode	*ConfManager::ManageDel(TiXmlNode *pCurrentContainer)
{
  string	sName;
  string	sElem;
  int		i;
  tStringVector::iterator	itIterator;


  cout << endl << "ici" << endl;
  sName = MyAttribute(pCurrentContainer->ToElement(), "name");
  sElem = MyAttribute(pCurrentContainer->ToElement(), "elem");
  if (sElem != "")
    {
      for (i = 0, itIterator = _mListData[sName].begin();
	   itIterator != _mListData[sName].end() && i < atoi(sElem.c_str()) - 1;
	   i++, itIterator++)
	;
      _mListData[sName].erase(itIterator);
    }
  else
    {
      _mSimpleData.erase(sName);
      _mListData.erase(sName);
    }
  return pCurrentContainer->NextSibling();
}

void	ConfManager::DumpToMemory(TiXmlNode *pCurrentContainer)
{
  if (!pCurrentContainer)
    return;

  int		iTypeContainer;

  pCurrentContainer = pCurrentContainer->FirstChild(); // skip DOCUMENT
  while (pCurrentContainer)
    {
      iTypeContainer = pCurrentContainer->Type();
      if (iTypeContainer == TiXmlNode::ELEMENT)
	{
	  int		i;
	  int		iStop;	// use to break search faster
	  string	sElement;

	  sElement = pCurrentContainer->ValueStr();
	  for (i = 0, iStop = 0; !iStop && i < NB_CONTAINER; i++)
	    {
	      iStop = 0;
	      if (InsensitiveCmp(_Container[i].sContainer, sElement))
		{
		  pCurrentContainer = (this->*_Container[i].fct)(pCurrentContainer);
		  iStop = 1;
		}
	    }
	  if (!iStop)
	    {
	      // mettre un msg d'erreur comme quoi la balise n'est pas geree
	      pCurrentContainer = pCurrentContainer->NextSibling();
	    }
	}
      else
	pCurrentContainer = pCurrentContainer->NextSibling(); // continue file analize's
    }
}



//
// Public members
//



ConfManager::ConfManager(char **av, const char &ConfFile)
{
  init_fct_ptr();
  // appeler la fonction qui gere les options (char **av)
  Reload(&ConfFile);
}

ConfManager::~ConfManager()
{
}

int	ConfManager::Clear()
{
  _mSimpleData.clear();
  _mListData.clear();
  return true;
}

int	ConfManager::Reload(string sConfFile)
{
  Clear();
  if (sConfFile == "")
    sConfFile = _LoadedFile;
  else
    _LoadedFile = sConfFile;
  Load(sConfFile);
  return true;
}
