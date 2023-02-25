#pragma once

namespace GameEngineSpace 
{
	// Node Interface
	class Node 
	{  
	public:
		virtual bool run() = 0;
	};

	// children Node 관리
	class CompositeNode : public Node 
	{ 
	private:
		std::list<std::shared_ptr<Node>> children;

	public:
		const std::list<std::shared_ptr<Node>>& getChildren() const { return children; }

		void addChild(std::shared_ptr<Node> child) { children.emplace_back(child); }
	};

	// Action(무조건 true를 반환)
	class ActionNode : public Node
	{
	private:
		std::function<bool()> action;
	
	public:
		ActionNode(const std::function<bool()>& _action)
			: action(_action)
			{}

	public:
		bool run() override { return action(); }
	};

	class ConditionNode : public Node 
	{
	private:
		std::function<bool()> condition;

	public:
		ConditionNode(const std::function<bool()>& _condition)
			: condition(_condition)
			{}
	
	public:
		bool run() override;
	};

	// 단일 자식 노드를 가지는 노드  (looping, negation, and counting... 등 을 만들때 사용한다.)
	class DecoratorNode : public Node 
	{
	private:
		std::shared_ptr<Node> child;

	public:
		DecoratorNode(const std::shared_ptr<Node>& _child)
			: child(_child) 
			{}

	public:
		bool run() override 
		{
			// child 노드가 run 이 되기전에 수행해야할 작업
			before();

			// child run
			bool result = child->run();

			// child 노드가 run 된 후 수행해야할 작업
			after();
			return result;
		}

		virtual void before() = 0;
		virtual void after() = 0;
	};

	// 하나의 child가 성공하면 전체 run() 작업이 성공한다. 모든 child가 실패하면 fail
	class SelectorNode : public CompositeNode 
	{
	public:
		virtual bool run() override;
	};

	// 하나의 child가 실패하면 전체 run() 작업이 실패한다. 모든 child가 성공해야 결과는 성공
	class SequenceNode : public CompositeNode 
	{
	public:
		virtual bool run() override;
	};
}

