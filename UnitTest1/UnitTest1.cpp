#include "pch.h"
#include "CppUnitTest.h"

#include "../TaskODA/Triangle.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
	
		TEST_METHOD(TestGetCenter)
		{
			Triangle* tr1 = new Triangle();
			Point p1(2, 4);
			Point p2(9, 6);
			Point p3(7, 2);
			tr1->set(p1, p2, p3);
			
			Assert::IsTrue(tr1->getCenter() -> x==6 && tr1->getCenter()->y==4);
		}

		TEST_METHOD(TestBoundingBox) {
			Triangle tr;
			Point p1(1, 1);
			Point p2(3, 1);
			Point p3(2, 4);
			tr.set(p1, p2, p3);

			Point topLeft, bottomRight;
			tr.boundingBox(topLeft, bottomRight);

			Assert::AreEqual(topLeft.x,1.0);
			Assert::AreEqual(topLeft.y, 4.0);
			Assert::AreEqual(bottomRight.x, 3.0 );
			Assert::AreEqual(bottomRight.y, 1.0 );
		}
		
	};
}
