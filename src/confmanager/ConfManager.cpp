//
// ConfLoader.cpp for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Sat Oct 22 10:25:16 2005 Bigand Xavier
// Last update Tue Oct 25 18:00:38 2005 jean-philippe bosquer
//

#include "ConfManager.h"

ConfManager::ConfManager(char **av, const char &conf_file)
{
  _pConfFile = new TiXmlDocument(&conf_file);

  init_fct_ptr();
  if (_pConfFile->LoadFile())
    {
//       cout << "Chargement du fichier de configuration." << endl;
//       cout << "Traitement du fichier de configuration." << endl << endl;
//       _pConfFile->Print();
      DumpToMemory(_pConfFile);
//       cout << endl << "Traitement du fichier configuration termine." << endl;
//       cout << "Verification des parametres manquants." << endl;
      // verifier que la validite des parametres indispensable
    }
  else
    {
      // voir pour indiquer les erreurs de syntaxe ou autre
//       cout << "Erreur lors du chargement du fichier de configration." << endl;
//       cout << "Initialisation de la configuration de base." << endl;
    }
  delete _pConfFile;
}

ConfManager::~ConfManager()
{
}

void	ConfManager::init_fct_ptr()
{
  // Faire attention a ce que NB_CONTAINER soit correctement definie

  _Container[0].sContainer = "requiere";
  _Container[0].fct = &ConfManager::ManageRequiere;
  _Container[1].sContainer = "include";
  _Container[1].fct = &ConfManager::ManageInclude;
  _Container[2].sContainer = "set";
  _Container[2].fct = &ConfManager::ManageSet;
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
  string	value;
  TiXmlNode	*pChildContainer;

  pChildContainer = pCurrentContainer->FirstChild();
  value = (pChildContainer->ToText())->ValueStr();
  return pCurrentContainer->NextSibling();
}

TiXmlNode	*ConfManager::ManageSet(TiXmlNode *pCurrentContainer)
{
  string	sName;
  string	sValue;
  TiXmlNode	*pChildContainer;

  if (pCurrentContainer->ToElement())
    sName = (pCurrentContainer->ToElement())->Attribute("name");
  pChildContainer = pCurrentContainer->FirstChild();
  if (pChildContainer)
    sValue = (pChildContainer->ToText())->ValueStr();
  _mSimpleData[sName] = sValue;
  return pCurrentContainer->NextSibling();
}

TiXmlNode	*ConfManager::ManageList(TiXmlNode *pCurrentContainer)
{
  string	sName;

  sName = (pCurrentContainer->ToElement())->Attribute("name");
  for (pCurrentContainer = pCurrentContainer->NextSibling();
       pCurrentContainer && pCurrentContainer->ValueStr() == "add";
       pCurrentContainer->NextSibling())
    {
      cout << (pCurrentContainer->ToText())->ValueStr() << endl;
      _mListData[sName].push_back((pCurrentContainer->ToText())->ValueStr());
    }
  return pCurrentContainer;
}


TiXmlNode	*ConfManager::ManageDel(TiXmlNode *pCurrentContainer)
{
  // le erase est secure (ne plante pas si la clef n'existe pas)
  string	sClef;

  sClef = (pCurrentContainer->ToElement())->Attribute("name");
  _mSimpleData.erase(sClef);
  _mListData.erase(sClef);
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
	  int	i;
	  int	iStop;	// use to break search faster
	  string	sElement;
	  string	value;

	  sElement = pCurrentContainer->ValueStr();
	  for (i = 0, iStop = 0; !iStop && i < NB_CONTAINER; i++)
	    {
	      iStop = 0;
	      if (_Container[i].sContainer == sElement)
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
