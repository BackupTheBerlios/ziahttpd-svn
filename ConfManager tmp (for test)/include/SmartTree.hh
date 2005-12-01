//
// SmartTree.cpp for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Thu Dec 01 11:49:44 2005 Bigand Xavier
// Last update Thu Dec 01 16:27:50 2005 Bigand Xavier
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
  SmartTree		*_Child;
  SmartTree		*_Brother;

  void	FillData(TiXmlNode *pCurrentContainer, string &Name, map<string, string> &mAttributes);
  void	BuildTree(TiXmlNode *pCurrentContainer, string &Name, map<string, string> &mAttributes);

public:
  SmartTree();
  ~SmartTree();

  void	AddBrother(TiXmlNode *pCurrentContainer);
};

#endif // __SmartTree_H__
