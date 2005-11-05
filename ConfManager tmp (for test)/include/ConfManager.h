/*
** ConfLoader.h for  in
**
** Made by Bigand Xavier
** Login   <@epita.fr>
**
** Started on  Sat Oct 22 10:25:57 2005 Bigand Xavier
** Last update Sat Nov 05 12:13:07 2005 Bigand Xavier
*/

#ifndef __ConfManager_H__
#define __ConfManager_H__

#define TIXML_USE_STL // Force lib TinyXML to use STL

#include <iostream>
#include <map>
#include <vector>
#include <cctype>	// for using std::tolwer and std::toupper
#include <algorithm>	// for using std::transform (convert string case)
#include "tinyxml.h"

using namespace	std;

#define DEFAULT_FILE		"./conf/zia.conf"
#define NB_CONTAINER		5
#define	SINGLE_VALUE		0
#define	LIST_VALUE		1
#define	EXPR_VALUE		2
#define	EXPR_TRUE		"true"
#define	EXPR_FALSE		"false"

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
  tStringVector	_svListInclude;	// protect against multiple inclusion

  void		init_fct_ptr();	// in progress
  string	MyAttribute(TiXmlElement *pElement, string sAttribute);	// waiting for correcte NULL return and case insensitive version
  int		InsensitiveCmp(string sValue1, string sValue2);
  int		Load(string sConfFile);
  void		GetValues(TiXmlNode *pCurrentContainer, string &sValue, tStringVector &svValue); // in Progress, may replace ManageVar and ManageList



 protected:
  string			_LoadedFile;	// sav conf file path for reload
  map<string, string>		_mSimpleData;
  map<string, tStringVector>	_mListData;
  ManageContainer		_Container[NB_CONTAINER];

  TiXmlNode	*ManageRequiere(TiXmlNode *pCurrentContainer);	// in progress
  TiXmlNode	*ManageInclude(TiXmlNode *pCurrentContainer);	// in progress
  TiXmlNode	*ManageVar(TiXmlNode *pCurrentContainer);	// OK for assignation, not for read
  TiXmlNode	*ManageDel(TiXmlNode *pCurrentContainer);	// OK
  TiXmlNode	*ManageList(TiXmlNode *pCurrentContainer);	// OK
  void		DumpToMemory(TiXmlNode *pParent);		// OK



 public:
  ConfManager(char **av, const char &ConfFile = DEFAULT_FILE[0]);	// OK
  ~ConfManager();							// OK

  string	&GetSimpleString(string sVar) {return _mSimpleData[sVar];};	// OK
  tStringVector	&GetListVector(string sVar) {return _mListData[sVar];};		// OK
  int		SetSimpleString(string sVar, string sValue) {_mSimpleData[sVar] = sValue; return true;};	// OK
  int		SetListVector(string sVar, tStringVector Value) {_mListData[sVar] = Value; return true;};	// OK
  int		Clear();			// OK
  int		Reload(string sConfFile = "");	// OK
};

#endif // __ConfManager_H__
