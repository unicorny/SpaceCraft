#ifndef __DR_CORE_CUBIC_SPLINE__
#define __DR_CORE_CUBIC_SPLINE__

struct CORE_API DRCubicSpline
{
	explicit DRCubicSpline(DRVector3 _startPos, DRVector3 _endPos, DRVector3 _startVector, DRVector3 _endVector)
		: startPos(_startPos), endPos(_endPos), startVector(_startVector), endVector(_endVector)
	{}
	DRVector3 getPoint(float fTime);
	DRVector3 getPoint(float fTime, int rekursion);

	DRVector3 startPos;
	DRVector3 endPos;
	DRVector3 startVector;
	DRVector3 endVector;
};
//-----------------------------------------------------------------------------------------------------------------
class CORE_API DRRNS
{
public:
	DRRNS() :mMaxDistance(0.0f), mBuildSplineCalled(false) {}
	~DRRNS() {reset();}

	DRReturn addNode(DRVector3 _position);
	void     buildSpline();
	void reset() {mNodes.clear(); mMaxDistance = 0.0f;}

	DRVector3 getPoint(float fTime, int rekursion = 0);

private:
	// internal. Based on Equation 14 
	DRVector3 getStartVelocity(int index);
	// internal. Based on Equation 15 
	DRVector3 getEndVelocity(int index);
	struct Node
	{
		explicit Node(DRVector3 _position)
			: position(_position),
			  velocity(DRVector3(0.0f)),
			  distance(0.0f) {}
		
		DRVector3 position;
		DRVector3 velocity;
		DRReal	  distance;
	};
	std::vector<Node> mNodes;
	DRReal			  mMaxDistance;
	bool			  mBuildSplineCalled;
};


#endif //__DR_CORE_CUBIC_SPLINE__