/*
** ConfLoader.h for  in
**
** Made by Bigand Xavier
** Login   <@epita.fr>
**
** Started on  Sat Oct 22 10:25:57 2005 Bigand Xavier
// Last update Wed Oct 26 21:09:59 2005 
*/

#ifndef __ConfManager_H__
#define __ConfManager_H__

#define TIXML_USE_STL // Force lib TinyXML to use STL

#include <iostream>
#include <map>
#include <vector>
#include <tinyxml.hh>

using namespace	std;

#define DEFAULT_FILE		"./conf/zia.conf"
#define NB_CONTAINER		5


typedef	vector<string>		tStringVector;

class	ConfManager
{
 private:
  struct	ManageContainer
  {
    string	sContainer;
    // mettre le pointeur dans un enum pour le surchager
    TiXmlNode	*(ConfManager::*fct)(TiXmlNode *pCurrentContainer);
  };
  void	init_fct_ptr();	// in progress



 protected:
  TiXmlDocument			*_pConfFile;
  map<string, string>		_mSimpleData;
  map<string, tStringVector>	_mListData;
  ManageContainer		_Container[NB_CONTAINER];

  TiXmlNode	*ManageRequiere(TiXmlNode *pCurrentContainer);	// in progress
  TiXmlNode	*ManageInclude(TiXmlNode *pCurrentContainer);	// in progress
  TiXmlNode	*ManageSet(TiXmlNode *pCurrentContainer);	// OK
  TiXmlNode	*ManageDel(TiXmlNode *pCurrentContainer);	// OK
  TiXmlNode	*ManageList(TiXmlNode *pCurrentContainer);	// in progress
  void		DumpToMemory(TiXmlNode * pParent);	// OK



 public:
  ConfManager(char **av, const char &conf_file = DEFAULT_FILE[0]);
  ~ConfManager();

  string	&GetSimpleString(string sVar) {return _mSimpleData[sVar];};	// OK
  tStringVector	&GelListVector(string sVar) {return _mListData[sVar];};		// OK

  // A confirmer :
  // surcharger l'operateur [] pour faire "value = ConfManger[var];"
  // surcharger l'operateur = pour faire "ConfManager[var] = value;"
  // les surcharges doivent gere les string et les vector
};

#endif // __ConfManager_H__
