#include "stdafx.h"
#include "UIListCommonDefine.hpp"


FriendListItemInfo::FriendListItemInfo()
{

}

FriendListItemInfo::~FriendListItemInfo()
{


}

FriendListItemInfo::FriendListItemInfo(const FriendListItemInfo& obj)
{
	folder = obj.folder;
	empty  = obj.empty;
	id = obj.id;
	logo = obj.logo; 
	nick_name = obj.nick_name;
	description = obj.description;
	groupId = obj.groupId;
	goupName = obj.goupName; 
	mobile = obj.mobile;
	mail = obj.mail;
	post = obj.post;
	status = obj.status;
	tag = obj.tag;
}

void FriendListItemInfo::SetTag(UINT_PTR tag)
{
	this->tag = tag;
}

UINT_PTR FriendListItemInfo::GetTag()
{
	return tag;
}

FriendListItemInfo* FriendListItemInfo::Clone()
{
	return new FriendListItemInfo(*this);
}

namespace DuiLib
{

double CalculateDelay(double state)
{
    return pow(state, 2);
}

void Node::set_parent(Node* parent)
{
	parent_ = parent;
}

Node::Node()
	: parent_(NULL), linkNode_(NULL)
{}

Node::Node(NodeData t)
	: data_(t)
	, parent_(NULL), linkNode_(NULL)
{}

// Node::Node(const Node& obj)
// {
// 	
// }

Node::Node(NodeData t, Node* parent)
: data_ (t)
, parent_ (parent), linkNode_(NULL)
{}

Node::~Node() 
{
// 	for (int i = 0; i < num_children(); ++i)
// 		delete children_[i]; 
	Node* pNode = NULL;
	for (Children::iterator it = children_.begin(); it!= children_.end(); ++it)
	{
		pNode = it->second;
		delete pNode;
	}
	for (Children::iterator it = children.begin(); it != children.end(); ++it)
	{
		pNode = it->second;
		delete pNode;
	}
}

NodeData& Node::data()
{
	return data_;
}

int Node::num_children() const
{
	return static_cast<int>(children_.size() + children.size());
}

Node* Node::child(int i)
{
	//return children_[i];
// 	if (children.size() > i)
// 		return children[i];
	
	return NULL;
}

Node* Node::parent()
{
	return ( parent_);
}

Children& Node::get_children()
{
	return children;
}

Children& Node::get_children_()
{
	return children_;
}

//子节点的个数
bool Node::has_children() const
{
	return num_children() > 0 ;
}

bool Node::folder() const
{
	return data_.folder_;
}

//
void Node::add_child(Node* child)
{
	child->set_parent(this); 
	//children_.push_back(child); 
	if (child->data().pItemData->status == FRIEND_OFFLINE)
	
		//默认收到联系人的时候是没有状态的即离线的
		 children_.insert(make_pair(child->data().pItemData->id, child));
	else 
		children_.insert(make_pair(child->data().pItemData->id, child));
}

void Node::remove_allchild()
{
	children_.clear();
	children.clear();
}

void Node::remove_child(Node* child)
{
// 	Children::iterator iter = children_.begin();
// 	for (; iter < children_.end(); ++iter)
// 	{
// 		if (*iter == child) 
// 		{
// 			children_.erase(iter);
// 			return;
// 		}
// 	}
	if (child->data().pItemData->status == FRIEND_OFFLINE)
		children_.erase(child->data().pItemData->id);
	else
		children.erase(child->data().pItemData->id);
}

// 获取到最后一个
Node* Node::get_last_child()
{
// 	if (has_children())
// 	{
// 		return child(num_children() - 1)->get_last_child();
// 	}
	Node* pNode = NULL;
	if (has_children())
	{
		if (!children_.empty())
			 return children_.rbegin()->second->get_last_child();
		else
			return  children.rbegin()->second->get_last_child();
	}
	else 
		return this;
}
Node* Node::GetlinkNode()
{
	return linkNode_;
}

void Node::SetlinkNode(Node* node)
{
	linkNode_ = node;
//	node->SetlinkNode(this);
}
// 
// Node* Node::Clone()
// {
// 
// }

}