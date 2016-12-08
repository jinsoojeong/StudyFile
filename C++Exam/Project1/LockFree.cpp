#include <atomic>
#include <mutex>
#include <thread>

/*
2016�� 4�� 18�� ������
����Ư��

non-blocking programming

�����尣�� ������ ���� �� ����ȭ�� ������ "Lock_free" �ڷᱸ���� ���ؼ� �̷�� ����!!
���� ���� �����忡�� ���ÿ� ȣ�� ���� ������ ������ ���� �ð��� ��� �� ���� ȣ���� �Ϸ�Ǵ� �˰���!!
��Ƽ�����忡�� ���ÿ� ȣ���ص� ��Ȯ�� ���!! -> STL X ��Ƽ�����忡�� ����.
Non-Blocking �ٸ� �����尡 � ���¿� �ְ� ��� ���� ȣ���� �Ϸ�.
ȣ���� �浹�Ͽ��� ��� ��� �ϳ��� ���ڰ� �־, ���ڴ� Delay X

����� ����ȭ
Lock_free()
ADD(), Remove()�� lock_free.
wait_free�� �����ϸ� ���α׷��� ����������.
Contains()�� Wait-free�� ����.

CAS�� �Ѱ�
- �ѹ��� �ϳ��� ���� �ۿ� �ٲ��� ���Ѵ�. -> ���� ���� ���ÿ� ���� �� ���� ����.
����, ���ø� �ؾ� �ϴ� ������ �پ�� �Ѵ�.(����å�� ��� �Ѵ�.) -> marking�� �ʿ��ϴ�.
Remove�� �����͵� �ٲ�����ϰ�, marking�� ���� Ȯ���ؾ� �Ѵ� -> ��� CAS�� ���ÿ� 2�� Ȯ�� �� ��??
���������� pred, curr�� �ٸ� �����尡 �������� �ƴ��� �˾ƾ� �Ѵ�. -> ���� �� CAS �ʿ�??

CAS�� ������ ��� �߸��°� ?? -> next�� marked�� �ϳ��� ���ĺ���!! ��Ƽ CAS ����!!
CAS(oldmark, mark, oldnext, next) -> CAS �ϳ����� �� �� �ۿ� �ȵǴµ� ��� ?! -> next �ʵ� �߿� ���� �κ��� marked�� �̿��غ���.
next�ʵ�� 32��Ʈ. new�� �� ���� 4�� ����� �Ҵ� �Ѵ�. CDB ������... �� �� 2���� 00���� ä���� �ִ�.(�ƹ��� �ǹ̰� ���� ��Ʈ�̴�.) �̰��� ����ϸ� �ȴ�!!

non-blocking �ڷᱸ��
1. lock ���� -> �� ���� atomic operation���� ��� ���� ������ ��!!
2. atomic operation - read, write, CAS.
CAS - �� ���� �޸� �ϳ��� ���� ����. �����ؾ� �� ���� ���� ���� ����?
3. ��å : ������ �� ��� �������� �� ������ ������.

*/

#include <iostream>
#include <mutex>
#include <atomic>
#include <vector>
#include <thread>
#include <concurrent_queue.h>

using namespace std;
using namespace concurrency;



class CNODE {
public:
	int key;
	int next_marked; // �ּ� + ��ũ

public:
	CNODE() {
		next_marked = 0;
	}
	CNODE(int num) {
		key = num;
		next_marked = 0; // NULL + FALSE
	}
	~CNODE() {
	}

	void SetKey(int n) { key = n; }
	void SetNext(CNODE *other) {
		next_marked = reinterpret_cast<int>(other) & 0xfffffffe;
	}

	CNODE* GetNextAddr() {
		return reinterpret_cast<CNODE*>(next_marked & 0xfffffffe);
	}
	CNODE* GetNextMarkable(bool *marked) {
		*marked = ((next_marked & 1) == 1);
		return reinterpret_cast<CNODE*>(next_marked & 0xfffffffe);
	}
	bool CAS(int old_v, int new_v) { return atomic_compare_exchange_strong(reinterpret_cast<atomic_int *>(&next_marked), &(old_v), new_v); }
	bool CAS(CNODE *old_node, CNODE* new_node, bool old_mark, bool new_mark) {
		int old_v = reinterpret_cast<int>(old_node);
		if (true == old_mark) old_v = old_v | 1;
		else old_v = old_v & 0xFFFFFFFE;

		int new_value = reinterpret_cast<int>(new_node);
		if (new_mark) new_value = new_value | 1;
		else new_value = new_value & 0xfffffffe;

		return CAS(old_v, new_value);
	}
	bool TryMarked(CNODE *next) {
		int old_value = reinterpret_cast<int>(next) & 0xfffffffe;
		int new_value = old_value | 1;
		return CAS(old_value, new_value);
	}
};

class CFREE_LIST {
	CNODE* free_list;
	CNODE* reuse_list;

	CNODE sentinel_1, sentinel_2;

public:
	CFREE_LIST() {
		sentinel_1.next_marked = 0;
		sentinel_1.key = 0x70000000;
		free_list = &sentinel_1;

		sentinel_2.next_marked = 0;
		sentinel_2.key = 0x70000000;
		reuse_list = &sentinel_2;
	}
	~CFREE_LIST() {

	}
	void clear() {
		CNODE* temp = free_list;
		free_list = reuse_list;
		reuse_list = temp;
	}
	CNODE* GetNode(int key) {
		if (reuse_list->GetNextAddr() == nullptr) return (new CNODE(key));
		while (true) {
			CNODE *temp = reuse_list;
			CNODE *next = temp->GetNextAddr();
			CNODE *succ = next->GetNextAddr();

			//��ų� ��������� lock_free�� �����ؾ� �Ѵ�.
				if (temp->CAS(next, succ, true, false)) // free_list�� �ִ� ���� ��ŷ �Ǿ��� ����̱� ������ �����ϱ� ���ؼ��� true�� false�� �ٲ��־�� �Ѵ�.
					return temp;
		}

	}
};

concurrency::concurrent_queue<CNODE*> free_queue;
CNODE *free_list = nullptr;
mutex glock;

class CSET {
	CNODE head, tail;
public:
	CSET() {
		head.key = 0x80000000;
		tail.key = 0x7FFFFFFF;
		head.SetNext(&tail);
	}
	~CSET() {

	}
	void Init() {
		CNODE* ptr;
		while (head.GetNextAddr() != &tail) {
			ptr = head.GetNextAddr();
			head.SetNext(head.GetNextAddr()->GetNextAddr());
			delete ptr;
		}
	}

	/*bool validate(CNODE *pred, CNODE *curr) {
	��ũ�� ���ִ� ���� ���� �� ����. ����, ���� ���� ��� �� �����ϱ� ������
	CAS�� �ֱ� ������ �޸� �񱳵� �� �ʿ䰡 ����.
	return ((!pred->marked) && (!curr->marked) && (pred->next == curr));
	}*/

	void Find(CNODE **pred, CNODE **curr, int key) {
		CNODE *succ;
	fail_retry:
		*pred = &head;
		*curr = (*pred)->GetNextAddr();
		while (true) {
			bool curr_is_removed;
			succ = (*curr)->GetNextMarkable(&curr_is_removed);
			while (curr_is_removed) {
				if (false == (*pred)->CAS((*curr), succ, false, false)) goto fail_retry;
				(*curr) = succ;
				succ = (*curr)->GetNextMarkable(&curr_is_removed);
			}
			if ((*curr)->key >= key) return;
			//foward
			(*pred) = (*curr);
			(*curr) = succ;
		}

	}
	bool Add(int key) {
		while (1) {
			CNODE* pred, *curr;
			Find(&pred, &curr, key);
			if (curr->key == key) return false;
			else {
				CNODE *node = new CNODE(key);
				auto free_node = free_queue.unsafe_begin();
				if (free_node == free_queue.unsafe_end())
					node = new CNODE(key); // free_queue.GetNode(key);
				else {
					(*free_node)->key = key;
					(*free_node)->next_marked = 0;
					node = (*free_node);
					if (!free_queue.try_pop((*free_node))) {
						delete node;
						continue;
					}
				}
				node->SetNext(curr);
				if (pred->CAS(curr, node, false, false))
					return true;
				else { delete node; continue; }
			}
		}
	}
	bool Remove(int key) {
		CNODE *pred, *curr;
		bool snip;
		while (1) {
			Find(&pred, &curr, key);
			if (curr->key != key) { return false; }
			else {
				CNODE *succ = curr->GetNextAddr();
				snip = curr->TryMarked(succ);
				if (!snip) continue;
				pred->CAS(curr, succ, false, false); // find���� ��ŷ �� ��带 ���������Ƿ�, �����ϵ� ���� ��°.

													 /*memory leak �ذ� ���?!
													 Lazy�� ����. free_list�� ���� Ȥ�� shared_ptr
													 free_list�� ��Ÿ�ӽ� delete timing�� ��� ��ƴ�.
													 shared_ptr�� c++11���� ������ ���� ���� �����÷ο��� ������ �ӵ��� ������ ���ϴ�.

													 free_list�� ������ �ʿ��ϴ�!!
													 ������ lazy���� ��� �ߴ� ����� lock�� �� �ִ�. -> blocking���� �߻��ϴ� ������ �����!!
													 non_blocking free_list�� �ʿ��ϴ�!! -> lock_free�� �����ؾ� �Ѵ�!! -> LF_QUEUE �ʿ�!!!
													 �� �� free_list���� delete���ٴ� �޸𸮸� ������ �ϴ� ���� ���� ��� ����!! -> ��� �� �� �ΰ�?!

													 free_queue.push(curr);*/

				return true;
			}
		}
	}
	bool Contains(int key) {
		bool curr_is_removed;
		CNODE *curr = &head;
		while (curr->key < key) curr = curr->GetNextMarkable(&curr_is_removed);
		return ((curr->key == key) && (!curr_is_removed));
	}

	CNODE* begin() { return &head; }
	CNODE* end() { return &tail; }
};
CSET cSet;
const auto NUM_TEST = 4000000;
const auto KEY_RANGE = 1000;
void List_ThreadFunc(int num_thread) {
	int key;
	for (auto i = 0; i < NUM_TEST / num_thread; ++i) {
		switch (rand() % 3) {
		case 0:
			key = rand() % KEY_RANGE;
			cSet.Add(key);
			break;
		case 1:
			key = rand() % KEY_RANGE;
			cSet.Remove(key);
			break;
		case 2:
			key = rand() % KEY_RANGE;
			cSet.Contains(key);
			break;
		default:
			cout << "Error \n";
			exit(-1);
		}
	}

}
int main()
{
	std::vector<std::thread*> v_threadlist;
	v_threadlist.reserve(16);

	free_list = new CNODE(0x7fffffff);
	for (auto i = 1; i <= v_threadlist.capacity(); i *= 2) {
		auto start_time = std::chrono::high_resolution_clock::now();
		cSet.Init();
		free_queue.clear();
		for (auto j = 0; j < i; ++j) {
			v_threadlist.push_back(new std::thread{ List_ThreadFunc, i });
		}

		for each (auto th in v_threadlist)
		{
			th->join();
			delete th;
		}
		auto end_time = std::chrono::high_resolution_clock::now() - start_time;
		v_threadlist.clear();

		cout << "Thread ���� : " << i;
		cout << " , Time : " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time).count() << "msecons" << endl;

		CNODE *ptr = cSet.begin()->GetNextAddr();
		for (auto i = 0; i < 10; ++i) {
			cout << ptr->key << " ";
			ptr = ptr->GetNextAddr();
		}
		cout << endl;
	}
	system("pause");
}