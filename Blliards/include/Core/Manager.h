#pragma once
#include <./Macro.h>
#include <vector>
#include <list>
#include <boost/range/algorithm.hpp>
#include <boost/shared_ptr.hpp>
namespace rinfw{
namespace core{

template<typename T>
class Manager {
public:
	bool Attach(SharedPtr<T> ptr) {
		SharedListPtr<T>::iterator it = boost::find_if(this->data, [ptr](SharedPtr<T> &v) {return v.get() == ptr.get(); });
		if (it != this->data.end())
			return false;
		this->data.push_back(ptr);

		return true;
	}
	Manager &Remove(T *data) {
		if (!data)
			return *this;
		this->data.remove_if(
			[data](auto &v)->bool {
			if (v == data) {
				v->Release();
				return true;
			}
			return false;
		});
		return *this;
	}
	virtual bool Init() { return true; }
	virtual bool Release() { return true; }
	virtual bool Update() {return true;}
	virtual bool Draw(){return true;}

	void ObjectUpdate(){
		this->data.remove_if(
			[](auto &v)->bool {
			return v != null && !v->Update();
		});
	}
protected:
	SharedListPtr<T> data;
private:
};

template<typename T>
class Link {
public:
	Link() {
		this->ptr[0] = nullptr;
		this->ptr[1] = nullptr;
	}
	Link(T* a, T* b) {
		this->ptr[0] = a;
		this->ptr[1] = b;
	}
	T* getLeft() {
		return this->ptr[0];
	}
	T* getRight() {
		return this->ptr[1];
	}
protected:
private:
	T* ptr[2];
};

template<typename T>
class LinkGroup {
public:
	LinkGroup() {

	}
	LinkGroup(std::list<T*> links) {
		this->links = links;
	}
	std::list<T*> getLinks() {
		return this->links;
	}
	bool Attach(T* ptr) {
		if (ptr = nullptr) {
			return false;
		}
		this->links.push_back(ptr);
		return true;
	}
	void Release() {
		for (auto &v : this->links) {
			v->Release();
		}
	}

private:
	std::list<T*> links;
};

class Component {
public:
	virtual void Delete() = 0;
	virtual void Release() = 0;
	virtual void Create() = 0;
};

class GameObject : public Manager<Component> {
public:
	void Delete() {
		for (auto &v : this->data) {
			v->Delete();
		}
		delete this;
	}
	bool Release() {
		for (auto &v : this->data) {
			v->Release();
		}
		return true;
	}
	void Create() {
		for (auto &v : this->data) {
			v->Create();
		}
	}
};

class Scene : public Manager<GameObject> {
public:
	virtual  ~Scene(){}
	void Delete() {
		for (auto &v : this->data) {
			v->Delete();
		}
		delete this;
	}
	virtual bool Init(){return true;}
	virtual bool Release() {return true;}
	virtual bool Update() {return true;}

	void Create(LinkGroup<GameObject> group) {
		for (auto &v : group.getLinks()) {
			v->Create();
		}
	}
};

class SequenceManager : public Manager<Scene> {
public:
	static SequenceManager &getInstance() {
		static SequenceManager ins;
		return ins;
	}
	~SequenceManager() {
	}
	void Delete() {
		for (auto &v : this->data) {
			v->Delete();
		}
	}
	bool setCurrent(Scene *current) {
		for (SharedPtr<Scene> &v: this->data) {
			Scene *s = (SharedPtr<Scene>(v)).get();
			if (s == current) {
				this->current = current;
				return true;
			}
		}
		return false;
	}
	bool setCurrent(SharedPtr<Scene> current) {
		for (SharedPtr<Scene> &v: this->data) {
			Scene *s = (SharedPtr<Scene>(v)).get();
			if (s == current.get()) {
				this->current = current.get();
				return true;
			}
		}
		return false;
	}
	Scene *getCurrent() {
		return this->current;
	}
	bool Change(Link<Scene> *link) {

		if (link->getLeft() == this->current) {
			SharedListPtr<Scene>::iterator it
				= boost::find_if(this->data, [link](SharedPtr<Scene> v) {return v.get() == link->getRight(); });
			if (it == this->data.end())
				return false;
			this->current = (*it).get();
			return true;
		}
		else if (link->getRight() == this->current) {
			SharedListPtr<Scene>::iterator it
				= boost::find_if(this->data, [link](SharedPtr<Scene> v) {return v.get() == link->getLeft(); });
			if (it == this->data.end())
				return false;
			this->current = (*it).get();
			return true;
		}
		return false;
	}

	bool Init() override {
		return (this->current != nullptr && this->current->Init());
	}
	bool Release() override {
		return (this->current != nullptr && this->current->Release());
	}
	bool Update() override {
		//後々scene移行処理を記述
		return (this->current != nullptr && this->current->Update());
	}
	bool Draw() override {
		return (this->current != nullptr && this->current->Draw());
	}
private:
	SequenceManager() {
		this->current = nullptr;
	}
	Scene* current;

};

}}