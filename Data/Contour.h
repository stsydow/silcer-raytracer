/*
 * Contour.h
 *
 *  Created on: 22.12.2011
 *      Author: st
 */

#ifndef CONTOUR_H_
#define CONTOUR_H_

#include "../Graphics/Drawable.h"
#include <list>
#include <assert.h>
#include "Coordinate.h"
#include "Plane.h"

class Contour: public Drawable {
public:
	Contour();
	void draw() const;
	virtual inline void draw() {
		draw();
	}

	bool insert(const Coordinate &c1, const Coordinate &c2);
	bool merge(const Contour &c);
	bool intersect(Plane &p, std::list<Coordinate> &intersections) const; 
	inline int size() const {
		int result = points.size();
		if (!isClosed && result > 0)
			result--;
		return result;
	}

	inline void check() const {
#if 0
		std::list<Coordinate>::const_iterator iter = points.begin();
		if (iter == points.end())
			return;
		Coordinate last = *iter;
		iter++;
		int i = 1;
		while (iter != points.end()) {
			if (last.distance(*iter) > 0.02) {
				printf("%d: %0.3f\n", i, last.distance(*iter));
				assert(0);
			}
			last = *iter;
			iter++;
			i++;
		}
#endif
	}

	inline void print(bool full = false) const {
		std::list<Coordinate>::const_iterator iter = points.begin();
		if (iter == points.end())
			return;
		Coordinate last = *iter;
		printf("[%0.6f,%0.6f,%0.6f]", (*iter)[0], (*iter)[1], (*iter)[2]);
		iter++;
		if (full) {
			while (iter != points.end()) {

				printf("[%0.4f,%0.4f,%0.4f]", (*iter)[0], (*iter)[1],
						(*iter)[2]);
				last = *iter;
				iter++;
			}
		} else {
			printf(" - %d", size());
		}
		const Coordinate &end = points.back();
		printf(" - [%0.6f,%0.6f,%0.6f]", end[0], end[1], end[2]);
		if (isClosed)
			printf(" closed");
	}

	inline std::list<Coordinate>::const_iterator begin() const {
		return points.begin();
	}
	inline std::list<Coordinate>::const_iterator end() const {
		return points.end();
	}
	inline bool is_closed() const{
		return isClosed;
	}

	Coordinate max, min;
private:
	std::list<Coordinate> points;
	bool isClosed;
};

#endif /* CONTOUR_H_ */
