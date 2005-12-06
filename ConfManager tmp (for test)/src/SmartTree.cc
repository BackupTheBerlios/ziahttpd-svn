//
// SmartTree.cc for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Thu Dec 01 11:51:59 2005 Bigand Xavier
// Last update Tue Dec 06 22:59:16 2005 Bigand Xavier
//

#include "SmartTree.hh"

void	SmartTree::FillData(TiXmlNode *pCurrentContainer, string &sName, string &sValue, map<string, string> &mAttributes)
{
  TiXmlElement		*pCurrentElement;
  TiXmlAttribute	*pCurrentAttribute;

  if ((pCurrentElement = pCurrentContainer->ToElement()))
    {
      sName = pCurrentElement->ValueStr();
      for (pCurrentAttribute = pCurrentElement->FirstAttribute();
	   pCurrentAttribute;
	   pCurrentAttribute = pCurrentAttribute->Next())
	mAttributes[pCurrentAttribute->Name()] = pCurrentAttribute->Value();
    }
  else if (pCurrentContainer->ToText())
    sValue = pCurrentContainer->ValueStr();
}

void	SmartTree::BuildTree(TiXmlNode *pBeginContainer, string &sName, string &sValue, map<string, string> &mAttributes)
{
  TiXmlNode	*pCurrentContainer;
  SmartTree	*pCurrent;

  if (pBeginContainer)
    {
      FillData(pBeginContainer, this->_sName, this->_sValue, this->_mAttributes);
      if ((pCurrentContainer = pBeginContainer->FirstChild()))
	{
	  this->_pChild = new SmartTree();
	  pCurrent = this->_pChild;
	  pCurrent->BuildTree(pCurrentContainer, pCurrent->_sName, pCurrent->_sValue, pCurrent->_mAttributes);
	}
      for (pCurrentContainer = pBeginContainer->NextSibling();
	   pCurrentContainer;
	   pCurrentContainer = pCurrentContainer->NextSibling())
	{
	  this->_pBrother = new SmartTree();
	  pCurrent = this->_pBrother;
	  pCurrent->BuildTree(pCurrentContainer, pCurrent->_sName, pCurrent->_sValue, pCurrent->_mAttributes);
	}
    }
}

SmartTree::SmartTree()
{
  _pChild = NULL;
  _pBrother = NULL;
  _pCurrent = this;
  _bFirst = true;
}

SmartTree::~SmartTree()
{
  delete _pChild;
  delete _pBrother;
}

void	SmartTree::AddBrother(TiXmlNode *pCurrentContainer)
{
  if (pCurrentContainer->Type() == TiXmlNode::ELEMENT)
    {
     if (!_pCurrent->_pChild && !_pCurrent->_sName.empty())
	{
	  _pCurrent->_pChild = new SmartTree();
	  _pCurrent = _pCurrent->_pChild;
	}
      FillData(pCurrentContainer, _pCurrent->_sName, _pCurrent->_sValue, _pCurrent->_mAttributes);
      if (pCurrentContainer->FirstChild())
	{
	  _pCurrent->_pBrother = new SmartTree();
	  _pCurrent = _pCurrent->_pBrother;
	  _pCurrent->BuildTree(pCurrentContainer->FirstChild(), _pCurrent->_sName, _pCurrent->_sValue, _pCurrent->_mAttributes);
	}
    }
}
