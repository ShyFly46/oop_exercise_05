/**
 * Савченко И.В.
 * М8О-208Б-19
 * https://github.com/ShyFly46/oop_exercise_05
 *
 * Вариант 5:
 * Ромб, стек
 **/


#include<iostream>
#include<memory>	// smart_ptr
#include<iterator>
#include<cstddef>   // ptrdiff_t
#include<utility>	// pair
#include<algorithm> // for_each

using namespace std;

template<typename SCALAR>
struct Rhombus{
	using vertex_t = pair<SCALAR, SCALAR>;
	vertex_t a, b, c, d;
};

using MyFigure = Rhombus<int>;

void FillRhombus(Rhombus<int> *fig, int h, int w){
	if(h < 0) h = -h;
	if(w < 0) w = -w;

	fig->a.first = 0;
	fig->a.second = h;

	fig->b.first = w;
	fig->b.second = 0;

	fig->c.first = 0;
	fig->c.second = -h;

	fig->d.first = -w;
	fig->d.second = 0;
}

void WriteRhombus(Rhombus<int> *fig){
	int h, w;
	cin >> h >> w;
	FillRhombus(fig, h, w);
}

void PrintRhombusPtr(const MyFigure* fig){
	cout << "Rho: "
	     << fig->a.second
		 << " "
		 << fig->b.first
		 << "\n";
}
void PrintRhombus(const MyFigure& fig){
	cout << "R: "
	     << fig.a.second
		 << " "
		 << fig.b.first
		 << "\n";
}

template<class T>
class CustomStack{
	struct Node{
		unique_ptr<T> *p; // pointer
		Node *d; // down
		Node *u; // up
		Node(unique_ptr<T>* data_ptr, Node *next = nullptr)
		  : p(data_ptr),
			d(next),
			u(nullptr)
		{}
		Node(T* data_ptr, Node *next = nullptr)
		  : p(new unique_ptr<T>(data_ptr)),
			d(next),
			u(nullptr)
		{}
		
		~Node(){
			delete p;
			if(d)
				d->u = u;
			if(u)
				u->d = d;
		}
	};

	Node *bottom,
		 *top;
public:
	class iterator;
	CustomStack() : bottom(nullptr), top(nullptr) {}
	void Push(T *data){
		if(!top){
			bottom = new Node(data);
			top = bottom;
			return;
		}

		top->u = new Node(data, top);
		top = top->u;
	}

	T* Top(){
		if(!top)
			throw 1;
		return top->p->get();
	}

	T* Pop(){
		if(!top)
			throw 1;
		Node *toPop = top;
		top = top->d;
		if(top){
			top->u = nullptr;
		} else {
			bottom = nullptr;
		}
		T* toRet = toPop->p->release();
		toPop->u = nullptr;
		toPop->d = nullptr;
		delete toPop;
		return toRet;
	}
	
	void insert(iterator& i, T* data){
		if(!i)
			throw 3;
		Node* u = i.m_node;
		Node* d = u->d;
		Node* newN = new Node(data);
		newN->u = u;
		newN->d = d;
		u->d = newN;
		if(d)
			d->u = newN;
	}
	
	void erase(iterator& i){
		if(!i)
			throw 3;
		iterator copy = i;
		++copy;
		if(i == top)
			top = i.m_node->d;
		if(i == bottom)
			bottom = i.m_node->u;
		delete i.m_node;
		i = copy;
	}
	
	bool IsEmpty(){
		return top;
	}

	iterator begin() {return iterator(top);}
	iterator end()   {return iterator(nullptr);}
};

template<typename T>
struct CustomStack<T>::iterator{
	friend class CustomStack<T>;
	
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;  // or also value_type*
    using reference         = T&;  // or also value_type&

	iterator(Node *node) : m_node(node) {}

	reference operator*() {return *(m_node->p->get());}
	pointer operator->() {return m_node->p;}
	iterator& operator++(){
		if(!m_node)
			throw 2;
		m_node = m_node->d;
		return *this;
	}
	iterator operator++(int){
		if(!m_node)
			throw 2;
		iterator toRet(m_node);
		m_node = m_node->d;
		return toRet;
	}
	operator pointer() {return m_node->p->get();}
	operator bool() {return m_node;}
	friend bool operator==(const iterator& a, const iterator& b)
		{return a.m_node == b.m_node;}
	friend bool operator!=(const iterator& a, const iterator& b)
		{return a.m_node != b.m_node;}
private:
	Node *m_node;
};

using CuStack = CustomStack<MyFigure>;

struct LessThan{
	int than;
	LessThan(int than) : than(than) {}
	bool operator()(const MyFigure& fig){
		int w = fig.a.second;
		int h = fig.b.first;
		int sur = w * h * 2;
		return sur < than;
	}
};

void help(){
	cout << "h - this message\n"
	     << "p - push\n"
		 << "r - remove by index\n"
		 << "s - show\n"
		 << "l - surface less than\n"
		 << "x - exit\n"
		 << "---\n";
}

int main(){
	CuStack myStack;
	MyFigure *rh_ptr;
	LessThan lt(0);
	
	char command;
	
	help();
	
	cout << "> ";
	while(cin >> command){
		switch(command){
			case 'p': // push
				rh_ptr = new MyFigure;
				WriteRhombus(rh_ptr);
				myStack.Push(rh_ptr);
				break;
			case 'r':
			{
				unsigned int index;
				cin >> index;
				CuStack::iterator del = myStack.begin();
				try{
					for(; index > 0; --index)
						++del;
					myStack.erase(del);
				} catch (int e){
					cout << "Failed to delete\n";
				}
				break;
			}
			case 's':
				for_each(myStack.begin(), myStack.end(), PrintRhombus);
				cout << "---\n";
				break;
			case 'h':
				help();
				break;
			case 'l':
				cin >> lt.than;
				cout << "Less than "
				     << lt.than
					 << " : "
					 << count_if(myStack.begin(), myStack.end(), lt)
					 << " figs\n";
				break;
			case 'x':
				return 0;
			default:
				break;
		}
		cout << "> ";
	}

}