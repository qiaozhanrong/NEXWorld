#ifndef VERTEXARRAY_H_
#define VERTEXARRAY_H_

#include <cstring>
#include <initializer_list>
#include <algorithm>
#include <common.h>
#include "opengl.h"
#include <debug.h>

class VertexFormat {
public:
	// Vertex attribute count
	unsigned int textureCount, colorCount, normalCount, coordinateCount;
	// Vertex attributes count (sum of all)
	int vertexAttributeCount;

	VertexFormat(): textureCount(0), colorCount(0), normalCount(0), coordinateCount(0), vertexAttributeCount(0) {}

	VertexFormat(unsigned int textureElementCount, unsigned int colorElementCount, unsigned int normalElementCount, unsigned int coordinateElementCount):
		textureCount(textureElementCount), colorCount(colorElementCount), normalCount(normalElementCount), coordinateCount(coordinateElementCount),
		vertexAttributeCount(textureElementCount + colorElementCount + normalElementCount + coordinateElementCount) {
		Assert(textureCount <= 3);
		Assert(colorCount <= 4);
		Assert(normalCount == 0 || normalCount == 3);
		Assert(coordinateCount <= 4 && coordinateCount >= 2);
	}
};

class VertexArray {
public:
	VertexArray(unsigned int maxVertexes, const VertexFormat& format):
		mMaxVertexes(maxVertexes), mVertexes(0), mFormat(format),
		mData(new float[mMaxVertexes * format.vertexAttributeCount]),
		mVertexAttributes(new float[format.vertexAttributeCount]) {}

	~VertexArray() {
		delete[] mData;
		delete[] mVertexAttributes;
	}

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	void clear() {
		memset(mData, 0, mMaxVertexes * mFormat.vertexAttributeCount * sizeof(float));
		memset(mVertexAttributes, 0, mFormat.vertexAttributeCount * sizeof(float));
		mVertexes = 0;
	}

	// Set texture coordinates
	void setTexture(size_t size, const float* texture) {
		Assert(size <= mFormat.textureCount);
		memcpy(mVertexAttributes, texture, size * sizeof(float));
	}

	void setTexture(std::initializer_list<float> texture) {
		setTexture(texture.size(), texture.begin());
	}

	// Set color value
	void setColor(size_t size, const float* color) {
		Assert(size <= mFormat.colorCount);
		memcpy(mVertexAttributes + mFormat.textureCount, color, size * sizeof(float));
	}

	void setColor(std::initializer_list<float> color) {
		setColor(color.size(), color.begin());
	}

	// Set normal vector
	void setNormal(size_t size, const float* normal) {
		Assert(size <= mFormat.normalCount);
		memcpy(mVertexAttributes + mFormat.textureCount + mFormat.colorCount, normal, size * sizeof(float));
	}

	void setNormal(std::initializer_list<float> normal) {
		setNormal(normal.size(), normal.begin());
	}

	// Add vertex
	void addVertex(const float* coords) {
		auto cnt = mFormat.textureCount + mFormat.colorCount + mFormat.normalCount;
		Assert(mVertexes * mFormat.vertexAttributeCount + cnt + mFormat.coordinateCount <= mMaxVertexes * mFormat.vertexAttributeCount);
		memcpy(mData + mVertexes * mFormat.vertexAttributeCount, mVertexAttributes, cnt * sizeof(float));
		memcpy(mData + mVertexes * mFormat.vertexAttributeCount + cnt, coords, mFormat.coordinateCount * sizeof(float));
		mVertexes++;
	}

	void addVertex(std::initializer_list<float> coords) {
		addVertex(coords.begin());
	}

	void addPrimitive(unsigned int size, std::initializer_list<float> d) {
		memcpy(mData + mVertexes * mFormat.vertexAttributeCount, d.begin(), size * mFormat.vertexAttributeCount * sizeof(float));
		mVertexes += size;
	}

	// Get current vertex format
	const VertexFormat& format() const { return mFormat; }
	// Get current vertex data
	const float* data() const { return mData; }
	// Get current vertex count
	int vertexCount() const { return mVertexes; }

private:
	// Max vertex count
	const unsigned int mMaxVertexes;
	// Vertex count
	int mVertexes;
	// Vertex array format
	VertexFormat mFormat;
	// Vertex array
	float* mData;
	// Current vertex attributes
	float* mVertexAttributes;
};

class VertexBuffer {
public:
	VertexBuffer(): mID(0), mVAO(0), mVertexes(0) {}
	VertexBuffer(VertexBuffer&& r): mID(0), mVAO(0), mVertexes(0) { swap(r); }
	/*VertexBuffer(VertexBufferID id, int vertexes, const VertexFormat& format):
		mID(id), mVertexes(vertexes), mFormat(format) {}*/
	explicit VertexBuffer(const VertexArray& va, bool staticDraw = true);
	~VertexBuffer() { destroy(); }

	VertexBuffer& operator=(VertexBuffer&& r) {
		swap(r);
		return *this;
	}

	// Is empty
	bool empty() const {
		if (mID == 0) {
			Assert(mVertexes == 0);
			return true;
		}
		return false;
	}
	// Upload new data
	void update(const VertexArray& va, bool staticDraw = true);
	// Swap
	void swap(VertexBuffer& r) {
		std::swap(mID, r.mID);
		std::swap(mVAO, r.mVAO);
		std::swap(mVertexes, r.mVertexes);
		std::swap(mFormat, r.mFormat);
	}
	// Render vertex buffer
	void render() const;
	// Destroy vertex buffer
	void destroy() {
		mFormat = VertexFormat();
		if (empty()) return;
		if (!OpenGL::coreProfile()) {
			glDeleteBuffersARB(1, &mID);
		} else {
			glDeleteVertexArrays(1, &mVAO);
			glDeleteBuffers(1, &mID);
		}
		mVertexes = mID = mVAO = 0;
	}

private:
	// Buffer ID
	VertexBufferID mID, mVAO;
	// Vertex count
	int mVertexes;
	// Buffer format
	VertexFormat mFormat;
};

#endif // !VERTEXARRAY_H_
