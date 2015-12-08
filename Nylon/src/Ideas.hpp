//Some basic ideas for classes like Fibers.

#include <vector>
#include <string>

//Actual fiber
class Fiber
{

};

class FiberGraphNode;

typedef std::vector<FiberGraphNode> TFiberGraphNodes;
class FiberGraphNode
{
	FiberGraphNode* pParent;
	TFiberGraphNodes pChildren;
#if FIBER_DEBUG_MODE
	std::string jobName;
#endif
};

