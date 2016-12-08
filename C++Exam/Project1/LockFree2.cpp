#include <atomic>
#include <mutex>
#include <thread>

class LFNODE {
public:
	int key;
	LFNODE *next;

	LFNODE(int v) {
		key = v;
		next = nullptr;
	}

	LFNODE() {
		next = nullptr;
	}

	bool CAS(LFNODE *old_node, LFNODE *new_node, bool oldMark, bool newMark) {
		int oldvalue = reinterpret_cast<int>(old_node);
		if (oldMark) oldvalue = oldvalue | 0x1;
		else oldvalue = oldvalue & 0xFFFFFFFE;

		int newvalue = reinterpret_cast<int>(new_node);
		if (newMark) newvalue = newvalue | 0x1;
		else newvalue = newvalue & 0xFFFFFFFE;
		return CAS(oldvalue, newvalue);
	}

	bool CAS(int oldValue, int newValue) {
		return std::atomic_compare_exchange_strong(reinterpret_cast<std::atomic<int>*>(&next), &oldValue, newValue);
	}

	LFNODE *GetNext() {
		int temp = reinterpret_cast<int>(next);
		return reinterpret_cast<LFNODE*>(temp & 0xFFFFFFFE);
	}

	LFNODE *GetNextMark(bool *deleted) {
		int temp = reinterpret_cast<int>(next);
		*deleted = (temp & 0x1) != 0;
		return reinterpret_cast<LFNODE*>(temp & 0xFFFFFFFE);
	}

	bool GetMark() {
		int temp = reinterpret_cast<int>(next);
		return (temp & 0x1);
	}

	bool AttemptMark(LFNODE *oldValue, bool newMark) {
		int temp = reinterpret_cast<int>(oldValue);
		temp = temp & 0xFFFFFFFE;

		int newValue = temp;
		if (newMark) newValue = newValue | 0x1;

		return CAS(temp, newValue);
	}
};

class LFLIST {
private:
	LFNODE head, tail;
public:
	LFLIST() {
		head.key = 0x80000000;
		tail.key = 0x7FFFFFFF;
		head.next = &tail;
	}

	void Init() {
		while (head.next != &tail) {
			LFNODE *temp = head.next;
			head.next = head.next->GetNext();
			delete temp;
		}
	}

	void Find(int key, LFNODE **pred, LFNODE **curr) {

		bool deleted = false;

	retry:
		*pred = &head;
		*curr = (*pred)->next;

		while (true) {
			LFNODE *succ = (*curr)->GetNextMark(&deleted);

			while (deleted) {
				if ((*pred)->CAS((*curr), succ, false, false) == false) {
					goto retry;
				}
			}

			if ((*curr)->key >= key) {
				return;
			}
			(*pred) = (*curr);
			(*curr) = succ;
		}
	}

	bool Add(int x)
	{
		LFNODE *pred, *curr;

		while (true) {

			Find(x, &pred, &curr);

			if (curr->key == x) {
				return false;
			}

			LFNODE *node = new LFNODE(x);
			node->next = curr;
			if (pred->CAS(curr, node, false, false)) {
				return true;
			}
			delete node;
		}
	}

	bool Remove(int x)
	{
		LFNODE *pred, *curr;

		while (true) {

			Find(x, &pred, &curr);

			if (curr->key != x) {
				return false;
			}

			LFNODE *succ = curr->GetNext();

			if (curr->AttemptMark(succ, true) == false) continue;

			pred->CAS(curr, succ, false, false);
			return true;
		}
	}

	bool Contains(int x)
	{
		LFNODE *curr = &head;
		while (curr->key < x) {
			curr = curr->GetNext();
		}
		return (curr->key == x && curr->GetMark() == false);
	}
};

int wmain()
{

	return 0;
}
