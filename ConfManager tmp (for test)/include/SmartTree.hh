//
// SmartTree.cpp for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Thu Dec 01 11:49:44 2005 Bigand Xavier
// Last update Tue Dec 06 22:58:55 2005 Bigand Xavier
//

#ifndef __SmartTree_H__
#define __SmartTree_H__

#include <iostream>
#include <map>

#include "tinyxml.hh"

using namespace std;

class	SmartTree
{
private:
  string		_sName;
  string		_sValue;
  map<string, string>	_mAttributes;
  SmartTree		*_pChild;
  SmartTree		*_pBrother;
  static SmartTree	*_pCurrent;
  static bool		_bFirst;

  void	FillData(TiXmlNode *pCurrentContainer, string &sName, string &sValue, map<string, string> &mAttributes);
  void	BuildTree(TiXmlNode *pCurrentContainer, string &sName, string &sValue, map<string, string> &mAttributes);

public:
  SmartTree();
  ~SmartTree();

  void	AddBrother(TiXmlNode *pCurrentContainer);
};

#endif // __SmartTree_H__
