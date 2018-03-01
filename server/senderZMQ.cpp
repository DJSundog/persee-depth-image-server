//
// Created by undef on 3/1/18.
//

#include <boost/cast.hpp>

#include "senderZMQ.h"


SenderZMQ::SenderZMQ(int port) : ctx(1), publisher(ctx, ZMQ_PUB) {
	publisher.bind("tcp://*:"+std::to_string(port));
	LOGI << "Start publishing on:" << port;
}

void SenderZMQ::send(const cv::Mat &mat) {
	OpenNI2SizeType sizeToSend = boost::numeric_cast<OpenNI2SizeType>(mat.total() * mat.elemSize());

	// data size
	OpenNI2NetHeader header = {
			sizeToSend,
			boost::numeric_cast<OpenNI2SizeType>(mat.cols),
			boost::numeric_cast<OpenNI2SizeType>(mat.rows),
			fovx,
			fovy,
			0
	};

	zmq::message_t msg(sizeof(OpenNI2NetHeader) + sizeToSend);
	memcpy(msg.data(), &header, sizeof(OpenNI2NetHeader));
	memcpy(static_cast<uint8_t*>(msg.data())+sizeof(OpenNI2NetHeader), mat.data, sizeToSend);
	publisher.send(msg);
}

void SenderZMQ::setFov(OpenNI2SizeType fx, OpenNI2SizeType fy) {
	fovx = fx;
	fovy = fy;
}
