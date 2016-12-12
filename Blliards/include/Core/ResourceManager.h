#pragma once




namespace rinfw {
namespace core {

template<typename T>
class ResourceManager{
public:
	static ResourceManager &getInstance() {
		static ResourceManager ins;
		return ins;
	}
	static bool Attach(boost::shared_ptr<T> &ptr) {
		if (ptr.get() == nullptr)
			return false;
		for (auto v : ResourceManager<T>::getInstance().res) {
			if (v.get() == ptr.get()) {
				return false;
			}
		}
		ResourceManager<T>::getInstance().res.push_back(ptr);
		return true;
	}
	static bool Attach(std::list<boost::shared_ptr<T>> &ptr) {
		for (auto i : ptr) {
			if (i.get() == nullptr)
				return false;
			for (auto v : ResourceManager<T>::getInstance().res) {
				if (v.get() == i.get()) {
					return false;
				}
			}
		}
		for (auto i : ptr) {
			ResourceManager<T>::getInstance().res.push_back(i);
		}
		return true;
	}
	static bool Remove(boost::shared_ptr<T> &ptr) {
		if (ptr.get() == nullptr)
			return false;
		for (auto v : ResourceManager<T>::getInstance().res) {
			if (v.get() == ptr.get()) {
				ResourceManager<T>::getInstance().res.erase(v);
				return true;
			}
		}
		return false;
	}
	static bool Remove(std::list<boost::shared_ptr<T>> &ptr) {
		auto size = ptr.size();
		unsigned long cnt = 0;
		for (auto i : ptr) {
			if (i.get() == nullptr)
				return false;
			for (auto v : ResourceManager<T>::getInstance().res) {
				if (i.get() == v.get()) {
					cnt++;
					break;
				}
			}
		}
		if (size != cnt) {
			return false;
		}
		boost::remove_if(ResourceManager<T>::getInstance().res,[ptr](auto v) {
			for (auto u : ptr) {
				if (v.get() == u.get())
					return true;
			}
			return false;
		});
		return true;
	}
private:
	std::list<boost::shared_ptr<T>> res;
};

}
}