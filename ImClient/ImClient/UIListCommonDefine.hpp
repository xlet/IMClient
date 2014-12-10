#ifndef UILISTCOMMONDEFINE_HPP
#define UILISTCOMMONDEFINE_HPP

#include <math.h>

//struct FriendListItemInfo;//

#define  FRIEND_ONLINE		0x01
#define  FRIEND_OFFLINE		0x02
#define  FRIEND_LEAVE       0x04
#define  FRIEND_BUSY		0x08
#define  FRIEND_UNDISTURB   0x10
#define  FRIEND_HIDE        0x20



class FriendListItemInfo
{
public:
	FriendListItemInfo();
	~FriendListItemInfo();
	FriendListItemInfo(const FriendListItemInfo& obj);
	//分组展开或者不展开
	bool folder; 
	//Item是否为空
	bool empty; 
	//Id
	tString id;
	//左侧图标
	tString logo; 
	//昵称
	tString nick_name;
	//最右侧描述
	tString description;
	//组ID， 目前没用处
	tString  groupId;
	//组名称
	tString goupName; 
	//移动电话
	tString mobile;
	//邮箱
	tString mail;

	//职位
	tString post;

	// 好友的在线状态
	int     status;
	 
	FriendListItemInfo* Clone();
	// 
	void SetTag(UINT_PTR tag);
	UINT_PTR GetTag();
private:
	UINT_PTR  tag;   
};

namespace DuiLib
{
class Node;
typedef std::map<tString,Node*>	Children;

struct NodeData
{
	int level_;
	bool folder_;
	bool child_visible_;
	bool has_child_;
	///*CDuiString*/  tString text_;
	//好友列表中value表示的是用户的Id
	///*CDuiString*/  tString value; 
	CListContainerElementUI* list_elment_;
	FriendListItemInfo*    pItemData;
};

double CalculateDelay(double state);

class Node
{
public:
	Node();
	//Node(const Node& obj);
	explicit Node(NodeData t);
	Node(NodeData t, Node* parent);
	~Node();
	NodeData& data();
	int num_children() const;
	Node* child(int i);
	Node* parent();
	bool folder() const;
	bool has_children() const;
	void add_child(Node* child);
	void remove_child(Node* child);
	void remove_allchild();
	Node* get_last_child();
	Children& get_children();
	Children& get_children_();
	Node* Clone();
	Node*  GetlinkNode();
	void  SetlinkNode(Node* node);
private:
	void set_parent(Node* parent);

private:
//	typedef std::vector <Node*>	Children;
	Children	children_; // 不在线的好友
	Children	children ; // 在线的，包括隐身离线繁忙的
	Node*		parent_;

	NodeData    data_;
	Node*       linkNode_;
};

} // DuiLib

#endif // UILISTCOMMONDEFINE_HPP