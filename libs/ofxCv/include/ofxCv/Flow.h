#pragma once

#include "ofxCv.h"

#pragma mark FLOW 
namespace ofxCv {
	
	using namespace cv;
	
	class Flow {
	public:
		Flow();
		~Flow();
		
		//call these functions to calculate flow on sequential images.
		//After this call the flow field will be populated and
		//subsequent calls to getFlow() will be updated
		
		//call with two contiguous images
		void calcOpticalFlow(ofBaseHasPixels& lastImage, ofBaseHasPixels& currentImage);
		void calcOpticalFlow(ofPixelsRef lastImage, ofPixelsRef currentImage);
		
		//call with subsequent images to do running optical flow. 
		//the Flow class internally stores the last image for convenience
		void calcOpticalFlow(ofBaseHasPixels& nextImage);
		void calcOpticalFlow(ofPixelsRef nextImage);
		
		void draw();
		void draw(float x, float y);
		void draw(float x, float y, float width, float height);
		void draw(ofRectangle rect);
		
	protected:
		ofImage last, curr;
		bool hasFlow;
		
		//specific flow implementation
		virtual void calcFlow() = 0;
		//specific drawing implementation
		virtual void drawFlow(ofRectangle r) = 0;
	};
	
	//there are two implementations of Flow
	//use Farneback for a dense flow field, 
	//use PyrLK for specific features
	
	//see http://opencv.willowgarage.com/documentation/cpp/motion_analysis_and_object_tracking.html
	//for more info on the meaning of these parameters
	
#pragma mark FLOW - PYR LK
	class FlowPyrLK : public Flow {
	public:
		FlowPyrLK();
		~FlowPyrLK();
		
		//flow parameters
		void setMinDistance(int minDistance);		
		void setWindowSize(int winsize);
		
		//feature finding parameters
		void setMaxLevel(int maxLevel);
		void setMaxFeatures(int maxFeatures);
		void setQualityLevel(float qualityLevel);
		
		//returns tracking features for this image
		vector<ofPoint> getFeatures();
		
	protected:
		
		void drawFlow(ofRectangle r);
		void calcFlow();
		
		vector<cv::Point2f> prevPts, nextPts;
		
		//LK feature finding parameters
		int windowSize;
		int maxLevel;
		int maxFeatures;
		float qualityLevel;
		
		//min distance for PyrLK 
		int minDistance;
	};
	
#pragma mark FLOW FARNEBACK	
	
	class FlowFarneback : public Flow {
	public:
		
		FlowFarneback();
		~FlowFarneback();
		
		//see http://opencv.willowgarage.com/documentation/cpp/motion_analysis_and_object_tracking.html
		//for a description of these parameters
		
		void setPyramidScale(float scale);
		void setNumLevels(int levels);
		void setWindowSize(int winsize);
		void setNumIterations(int interations);
		void setPolyN(int polyN);
		void setPolySigma(float polySigma);
		void setUseGaussian(bool gaussian);
		
		ofVec2f getTotalFlow();
		ofVec2f getAverageFlow();		
		ofVec2f getFlowOffset(int x, int y);
		ofVec2f getFlowPosition(int x, int y);
		ofVec2f getTotalFlowInRegion(ofRectangle region);
		ofVec2f getAverageFlowInRegion(ofRectangle region);
		
	protected:
		cv::Mat flow;
		
		void drawFlow(ofRectangle rect);
		void calcFlow();
		
		float pyramidScale;
		int numLevels;
		int windowSize;
		int numIterations;
		int polyN;
		float polySigma;
		bool farnebackGaussian;
	};
	
}