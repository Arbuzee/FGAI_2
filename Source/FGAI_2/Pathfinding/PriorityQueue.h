#pragma once

template <typename PQElement>
struct TPriorityQueueNode {
	PQElement Element;
	float Priority;
 
	TPriorityQueueNode() : Priority(0)
	{
	}
 
	TPriorityQueueNode(PQElement InElement, float InPriority)
	{
		Element = InElement;
		Priority = InPriority;
	}
 
	bool operator<(const TPriorityQueueNode<PQElement> Other) const
	{
		return Priority < Other.Priority;
	}
};

template<typename PQElement>
struct TPriorityQueue
{
private:
	TArray<TPriorityQueueNode<PQElement>> Array;

public:
	TPriorityQueue()
	{
		Array.Heapify();
	}

	PQElement Pop()
	{
		TPriorityQueueNode<PQElement> Node;
		Array.HeapPop(Node);
		return Node.Element;
	}

	void Push(PQElement Element, float Priority)
	{
		Array.HeapPush(TPriorityQueueNode<PQElement>(Element, Priority));
	}
 
	bool IsEmpty() const
	{
		return Array.Num() == 0;
	}
};
