//
// SmartTree.cc for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Thu Dec 01 11:51:59 2005 Bigand Xavier
// Last update Thu Dec 01 17:05:38 2005 Bigand Xavier
//

#include "SmartTree.hh"

void	SmartTree::FillData(TiXmlNode *pCurrentContainer, string &sName, map<string, string> &mAttributes)
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
}

void	SmartTree::BuildTree(TiXmlNode *pBeginContainer, string &sName, map<string, string> &mAttributes)
{
  TiXmlNode	*pCurrentContainer;
  SmartTree	*pCurrent;

  FillData(pBeginContainer, this->_sName, this->_mAttributes);
  if ((pCurrentContainer = pBeginContainer->FirstChild()))
    {
      this->_Child = new SmartTree();
      pCurrent = this->_Child;
      pCurrent->BuildTree(pCurrentContainer, pCurrent->_sName, pCurrent->_mAttributes);
    }
  for (pCurrentContainer = pBeginContainer;
       pCurrentContainer;
       pCurrentContainer = pCurrentContainer->NextSibling())
    {
      this->_Brother = new SmartTree();
      pCurrent = this->_Brother;
      pCurrent->BuildTree(pCurrentContainer, pCurrent->_sName, pCurrent->_mAttributes);
    }
}

SmartTree::SmartTree()
{
  _Child = NULL;
  _Brother = NULL;
}

SmartTree::~SmartTree()
{
  delete _Child;
  delete _Brother;
}

void	SmartTree::AddBrother(TiXmlNode *pCurrentContainer)
{
  SmartTree	*pCurrent;

  if (pCurrentContainer->ToElement())
    {

      for (pCurrent = this;
	   pCurrent->_Child && !pCurrent->_sName.empty();
	   pCurrent = pCurrent->_Child)
	;
      if (!pCurrent->_Child && !pCurrent->_sName.empty())
	{
	  pCurrent->_Child = new SmartTree();
	  pCurrent = pCurrent->_Child;
	}
      FillData(pCurrentContainer, pCurrent->_sName, pCurrent->_mAttributes);
      if (pCurrentContainer->FirstChild())
	{
	  pCurrent->_Brother = new SmartTree();
	  pCurrent = pCurrent->_Brother;
	  pCurrent->BuildTree(pCurrentContainer->FirstChild(), pCurrent->_sName, pCurrent->_mAttributes);
	}
    }
}
