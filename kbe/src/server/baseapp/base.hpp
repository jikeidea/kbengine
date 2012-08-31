/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef __BASE_H__
#define __BASE_H__
	
// common include	
#include "cstdkbe/cstdkbe.hpp"
#include "helper/debug_helper.hpp"
//#include "entityMailbox.hpp"
#include "pyscript/math.hpp"
#include "pyscript/scriptobject.hpp"
#include "entitydef/datatypes.hpp"	
#include "entitydef/entitydef.hpp"	
#include "entitydef/scriptdef_module.hpp"
#include "entitydef/entity_macro.hpp"	
#include "server/script_timers.hpp"		

//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{
class EntityMailbox;

namespace Mercury
{
class Channel;
}


class Base : public script::ScriptObject
{
	/** 子类化 将一些py操作填充进派生类 */
	BASE_SCRIPT_HREADER(Base, ScriptObject)	
	ENTITY_HEADER(Base)
public:
	Base(ENTITY_ID id, const ScriptDefModule* scriptModule, 
		PyTypeObject* pyType = getScriptType(), bool isInitialised = true);
	~Base();

	/** 
		是否存储数据库 
	*/
	INLINE bool hasDB()const ;
	INLINE void hasDB(bool has) ;

	/** 
		定义属性数据被改变了 
	*/
	void onDefDataChanged(const PropertyDescription* propertyDescription, 
			PyObject* pyData);

	/** 
		销毁cell部分的实体 
	*/
	bool destroyCellEntity(void);

	DECLARE_PY_MOTHOD_ARG0(pyDestroyCellEntity);
	
	/** 
		脚本请求销毁base实体 
	*/
	void destroyBase(void);

	DECLARE_PY_MOTHOD_ARG0(pyDestroyBase);
	
	/** 
		脚本获取mailbox 
	*/
	DECLARE_PY_GET_MOTHOD(pyGetCellMailbox);

	EntityMailbox* getCellMailbox(void)const;

	void setCellMailbox(EntityMailbox* mailbox);
	
	/** 
		脚本获取mailbox 
	*/
	DECLARE_PY_GET_MOTHOD(pyGetClientMailbox);

	EntityMailbox* getClientMailbox()const;

	void setClientMailbox(EntityMailbox* mailbox);

	/** 
		cellData部分 
	*/
	bool installCellDataAttr(PyObject* dictData = NULL);

	void createCellData(void);

	void destroyCellData(void);

	void addCellDataToStream(uint32 flags, MemoryStream* s);
	void addPersistentsDataToStream(uint32 flags, MemoryStream* s);

	PyObject* createCellDataDict(uint32 flags);

	INLINE PyObject* getCellData(void)const;
	
	/**
		请求cell部分将entity的celldata更新一份过来
	*/
	void reqBackupCellData();
	
	/** 
		写备份信息到流
	*/
	void writeBackupData(MemoryStream* s);
	void onBackup();

	/** 
		写存档信息到流
	*/
	void writeArchiveData(MemoryStream* s);

	/** 
		将要保存到数据库之前的通知 
	*/
	void onWriteToDB();
	void onCellWriteToDBCompleted();

	/**
		获取这个entity的client部分的属性放到流中
	*/
	void getClientPropertys(MemoryStream* s);

	/** 
		创建cell失败回调 
	*/
	void onCreateCellFailure(void);

	/** 
		创建cell成功回调 
	*/
	void onGetCell(Mercury::Channel* pChannel, COMPONENT_ID componentID);

	/** 
		丢失cell了的通知 
	*/
	void onLoseCell(Mercury::Channel* pChannel, MemoryStream& s);

	/** 
		备份cell数据
	*/
	void onBackupCellData(Mercury::Channel* pChannel, MemoryStream& s);

	/** 
		客户端丢失 
	*/
	void onClientDeath();

	/** 网络接口
		远程呼叫本entity的方法 
	*/
	void onRemoteMethodCall(Mercury::Channel* pChannel, MemoryStream& s);

	/** 
		销毁这个entity 
	*/
	void onDestroy(void);

	/** 
		为一个baseEntity在制定的cell上创建一个cellEntity 
	*/
	DECLARE_PY_MOTHOD_ARG1(createCellEntity, PyObject_ptr);

	/** 
		创建一个cellEntity在一个新的space上 
	*/
	DECLARE_PY_MOTHOD_ARG1(createInNewSpace, PyObject_ptr);

	/** 网络接口
		客户端直接发送消息给cell实体
	*/
	void forwardEntityMessageToCellappFromClient(Mercury::Channel* pChannel, MemoryStream& s);

protected:
	// 这个entity的客户端mailbox cellapp mailbox
	EntityMailbox*							clientMailbox_;			
	EntityMailbox*							cellMailbox_;

	// entity创建后，在cell部分未创建时，将一些cell属性数据保存在这里
	PyObject*								cellDataDict_;			

	// 是否是存储到数据库中的entity
	bool									hasDB_;					

	// 是否正在获取celldata中
	bool									isGetingCellData_;

	// 是否正在存档中
	bool									isArchiveing_;

	// 是否正在创建cell中
	bool									creatingCell_;

	// 是否已经创建了一个space
	bool									createdSpace_;
};

}


#ifdef CODE_INLINE
#include "base.ipp"
#endif

#endif
