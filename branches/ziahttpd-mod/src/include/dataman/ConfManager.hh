/*
** ConfLoader.h for  in
**
** Made by Bigand Xavier
** Login   <@epita.fr>
**
** Started on  Sat Oct 22 10:25:57 2005 Bigand Xavier
// Last update Tue Nov 22 10:36:27 2005 texane
*/

#ifndef __ConfManager_H__
#define __ConfManager_H__

#define TIXML_USE_STL // Force lib TinyXML to use STL

#include <iostream>
#include <map>
#include <vector>
#include <cctype>	// for using std::tolwer and std::toupper
#include <algorithm>	// for using std::transform (convert string case)
#include <dataman/tinyxml.hh>

using namespace	std;


#define DEFAULT_FILE		"./conf/zia.conf"
#define NB_CONTAINER		6
#define	SINGLE_VALUE		0
#define	LIST_VALUE		1
#define	EXPR_VALUE		2
#define	EXPR_TRUE		"true"
#define	EXPR_FALSE		"false"
#define EVAL_SIMPLE		0
#define EVAL_LOOP		1
#define OP_UNDEFINED		-1
#define OP_OR			0
#define	OP_AND			1

/// An useful typedef
typedef	vector<string>		tStringVector;

class	ConfManager
{
 private:
  struct	ManageContainer
  {
    string	sContainer;
    TiXmlNode	*(ConfManager::*fct)(TiXmlNode *pCurrentContainer);
  };
  tStringVector	_svListInclude;	// protect against multiple inclusion

  void		init_fct_ptr();	// in progress
  string	MyAttribute(TiXmlElement *pElement, string sAttribute);	// waiting for correcte NULL return and case insensitive version
  bool		InsensitiveCmp(string sValue1, string sValue2);	// OK, but can optimize with stricmp()?
  int		Load(string sConfFile);	// OK
  void		GetValues(TiXmlNode *pCurrentContainer, string &sValue, tStringVector &svValue); // OK, but juste add "expression var", "header var" and "List value"
  string	Eval_Expression(TiXmlNode *pCurrentContainer, bool *pbRes);	// OK, but juste add "<", ">", "<=" and ">=" comparator



 protected:
  string			_LoadedFile;	// sav conf file path for reload
  map<string, string>		_mSimpleData;
  map<string, tStringVector>	_mListData;
  ManageContainer		_Container[NB_CONTAINER];

  TiXmlNode	*ManageRequiere(TiXmlNode *pCurrentContainer);	// in progress
  TiXmlNode	*ManageInclude(TiXmlNode *pCurrentContainer);	// OK
  TiXmlNode	*ManageVar(TiXmlNode *pCurrentContainer);	// OK
  TiXmlNode	*ManageList(TiXmlNode *pCurrentContainer);	// OK
  TiXmlNode	*ManageEval(TiXmlNode *pCurrentContainer) {return ManageEval(pCurrentContainer, EVAL_SIMPLE, NULL);};	// OK
  TiXmlNode	*ManageEval(TiXmlNode *pCurrentContainer, int iFlag, bool *pbRes);	// OK, must test it, and some features depend to Eval_Expression()
  TiXmlNode	*ManageDel(TiXmlNode *pCurrentContainer);	// OK
  void		DumpToMemory(TiXmlNode *pParent);		// OK



 public:
  ConfManager();
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

/// \file
/// \brief This file contains the class ConfManager
///
/// She is based on the TinyXml lib which respects the RFC 3076

/// \class ConfManager
/// \brief This class is used to load a configuration
///
/// This class can load a configuration starting from a file of configuration
/// or since its own resources to have the configuration by default.
/// After the loading starting from a file it supplements the missing data or
/// on the contrary removes those which are not recognized by the server.



/// struct ManageContainer
///   {
///     string sContainer;
///     TiXmlNode *(ConfManager::*fct)(TiXmlNode *pCurrentContainer);
///  };

/// tStringVector _svListInclude;

/// \fn void ConfManager::init_fct_ptr()
/// \brief All is in the name
///
///

/// \fn string ConfManager::MyAttribute(TiXmlElement *pElement, string sAttribute)
/// \brief Return value of an attribute
///
/// The matching isn't case sensitive.

/// \fn int ConfManager::InsensitiveCmp(string sValue1, string sValue2)
/// \brief All is in the name, return 'true' or 'false'
///
/// It's a insensite comparasion between two string.
/// Return 'true' or 'false'.

/// \fn int ConfManager::Load(string sConfFile)
/// \brief All is in the name
///
///

/// \fn void ConfManager::GetValues(TiXmlNode *pCurrentContainer, string &sValue, tStringVector &svValue)
/// \brief This function fill all type of var
///
/// This function is recursive, she get a value and set it at all children vars.
/// She fill sValue and svValue. sValue can be "", and svValue can be unmodified
/// (you must verify the size).
/// The last (the oldest parent container) can be set, if you give him, but if
/// you don't want give her first children (he can be NULL).

/// \fn string ConfManager::Eval_Expression(TiXmlNode *pCurrentContainer, bool *pbRes)
/// \brief Fill pbRes (can be NULL) and return "true" or "false".
///
/// This function return (the string) and/or set (pbRes) with a result of one
/// "expression"
/// At this time, she can only do an "equal" or "diff" comparaison.
/// In futher she must convert 'string' in 'double' for the others comparaison
/// operator.



/// \fn string ConfManager::_LoadedFile;
/// \brief Contains last loaded file name

/// \fn map<string, string> ConfManager::_mSimpleData
/// \brief Contains all Simple Var with her keys

/// \fn map<string, tStringVector> ConfManager::_mListData
/// \brief Contains all List Var with her keys

/// \fn ManageContainer ConfManager::_Container[NB_CONTAINER]
/// \brief Contains pointer on the balises Manage function

/// \fn TiXmlNode *ConfManager::ManageRequiere(TiXmlNode *pCurrentContainer)
/// \brief All is in the name
///
/// Not implemented (Any effect for the moment).
/// Return the next container at the same level.

/// \fn TiXmlNode *ConfManager::ManageInclude(TiXmlNode *pCurrentContainer)
/// \brief All is in the name
///
/// Load an other file, it can't have loop inclusion.
/// Return the next container at the same level.

/// \fn TiXmlNode *ConfManager::ManageVar(TiXmlNode *pCurrentContainer)
/// \brief All is in the name
///
/// Return the next container at the same level.

/// \fn TiXmlNode *ConfManager::ManageList(TiXmlNode *pCurrentContainer)
/// \brief All is in the name
///
/// Return the next container at the same level.

/// \fn TiXmlNode *ConfManager::ManageEval(TiXmlNode *pCurrentContainer)
/// \brief All is in the name
///
/// Only call by DumpToMemory().
/// Return the next container at the same level.

/// \fn TiXmlNode *ConfManager::ManageEval(TiXmlNode *pCurrentContainer, int iFlag, bool *pbRes)
/// \brief All is in the name
///
/// This function isn't tested.
///
/// iFlag is used to know if we are in a "Loop" or not.
/// pbRes is fill with 'true' or 'false', he can be NULL.
/// Return the next container at the same level.

/// \fn TiXmlNode *ConfManager::ManageDel(TiXmlNode *pCurrentContainer)
/// \brief All is in the name
///
/// Remove a "Var", a "List", or one member of a "List".
/// Return the next container at the same level.

/// \fn void ConfManager::DumpToMemory(TiXmlNode *pParent)
/// \brief this function read the file
///
/// This function analyses the file and save variables in memory, she is also
/// call for excute "Do" block for a "Loop" or a "Eval".



/// \fn ConfManager::ConfManager(char **av, const char &ConfFile = DEFAULT_FILE[0])
/// \brief This is the constructor
///
/// He analyse the programme's line commande and load the configuration file, if
/// there is no file, he try too load the default file or even configuration by
/// default.
/// The line commande is not read actualy.

/// \fn ConfManager::~ConfManager()
/// \brief This is the destructor
///
/// He destroy all allocated objects

/// \fn string &ConfManager::GetSimpleString(string sVar)
/// \brief Returns the value which corresponds to the string
///
/// This function returns a string value which corresponds to the string passed
/// in parameter. The returned string can be wrong but not empty

/// \fn tStringVector &ConfManager::GetListVector(string sVar)
/// \brief returns the values which corresponds to the string
///
/// This function returns a string vector which contains values which
/// corresponds to the string passed in parameter.

/// \fn int ConfManager::SetSimpleString(string sVar, string sValue)
/// \brief Set sVar at sValue
///
/// This function associates the value sValue at the variable sVar.
/// Always returns true.

/// \fn int ConfManager::SetListVector(string sVar, tStringVector Value)
/// \brief returns the values which corresponds to the string
///
/// This function returns a string vector which contains values which
/// corresponds to the string passed in parameter.
/// Always returns true.

/// \fn int ConfManager::Clear();
/// \brief Destroy all data
///
/// This fonction clear all data.
/// Always returns true.

/// \fn int ConfManager::Reload(string sConfFile = "");
/// \brief Reload the configuration
///
/// This function reload the configuration. If there is no parameter the last
/// file which used for load the configuration is choose for the reload.
/// Always returns true.
