//
// ConfLoader.cpp for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Sat Oct 22 10:25:16 2005 Bigand Xavier
// Last update Sat Nov 05 12:29:52 2005 Bigand Xavier
//

#include "ConfManager.h"

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
}

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

void		ConfManager::GetValues(TiXmlNode *pCurrentContainer, string &sValue, tStringVector &svValue)
{
  if (pCurrentContainer->ToText()) // contient directement une simple valeur
    sValue = (pCurrentContainer->ToText())->ValueStr(); // can copy ""
  else // contient d'autres balises (comme une var deja declaree)
    {
      TiXmlNode	*pChildContainer;
      string	sName;
      string	sElement;	// type of element (var, list, ...)

      if (pCurrentContainer->ToElement())
	{
	  sName = MyAttribute(pCurrentContainer->ToElement(), "name");
	  sElement = pCurrentContainer->ValueStr();
	}
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
  TiXmlNode	*pChildContainer;
  TiXmlNode	*pNextContainer;
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
	  string	value;

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

string	ConfManager::MyAttribute(TiXmlElement *pElement, string sAttribute)
{
  const char		*tmp;	// for protect string to NULL return
  string		sCurrentAttribute;
  TiXmlAttribute	*pCurrentAttribute;
  int			iStop;

  transform(sAttribute.begin(), sAttribute.end(), sAttribute.begin(), tolower);
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
