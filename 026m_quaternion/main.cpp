#include <vcg/complex/complex.h>
#include <vcg/simplex/face/pos.h>
#include <iostream>
#include <common/ml_mesh_type.h>

void printP(const Point3m &p){
	std::cout << "(" 
			  << p.X() <<", "
			  << p.Y() <<", "
			  << p.Z() << ")"
			  << std::endl;
}

int main(){
	{
		Point3m p(10,10,10);
		Point3m z(0,0,10);
		vcg::Quaternionf qua(vcg::math::ToRad(90.0f),z);
		
		Point3m q = qua.Rotate(p);
		
		std::cout << q.X() << std::endl;
		std::cout << q.Y() << std::endl;
		std::cout << q.Z() << std::endl;
	}
	
	{
		Point3m orig(0,0,0);
		Point3m dir(10,0,0);
		vcg::Line3f line(orig, dir);
		
		Point3m p(-1000,10,0);
		Point3m q = line.ClosestPoint(p);
		
		std::cout << q.X() << std::endl;
		std::cout << q.Y() << std::endl;
		std::cout << q.Z() << std::endl;
		
		float a = vcg::Distance(line, p);
		std::cout << a << std::endl;
	}
	
	{
		vcg::Plane3f plate;
		
		Point3m a(0,0,10),b(10,0,10),c(0,10,10);
		plate.Init(a,b,c);
		std::cout << "offset:" << plate.Offset() << std::endl;
		
		Point3m p(10,10,0);
		Point3m q = plate.Projection(p);
		
		std::cout << q.X() << std::endl;
		std::cout << q.Y() << std::endl;
		std::cout << q.Z() << std::endl;
		
		float l = vcg::SignedDistancePlanePoint(plate, p);
		std::cout << l << std::endl;
	}
	{
		// Angle Test
		Point3m A(10,0,0);
		Point3m B(10,10,0);
		
		std::cout << "angle:" << vcg::Angle(A,B) << std::endl;
		std::cout << "angle:" << vcg::Angle(B,A) << std::endl;
		
		// A to B
		{
			vcg::Quaternionf qua(vcg::Angle(A,B),A^B);
			Point3m q = qua.Rotate(A);
			std::cout << q.X() << std::endl;
			std::cout << q.Y() << std::endl;
			std::cout << q.Z() << std::endl;
		}
		// B to A
		{
			vcg::Quaternionf qua(vcg::Angle(B,A),B^A);
			Point3m q = qua.Rotate(B);
			std::cout << q.X() << std::endl;
			std::cout << q.Y() << std::endl;
			std::cout << q.Z() << std::endl;
		}
		
	}
	{
		// Test
		std::cout << "-----------" << std::endl;
		Point3m y0(1,5,8),y1(10,10,10),z1(5,20,3);
		//vcg::Line3fN newYaxis(y0, y1);
		vcg::Line3f newYaxis(y0, y1-y0);
		Point3m z0 = newYaxis.ClosestPoint(z1);
		Point3m y01 = y1-y0;
		Point3m Y(0,1,0), Z(0,0,1);
		
		// quaternionでy軸合わせる
		{
			vcg::Quaternionf qua1(vcg::Angle(y01,Y),y01^Y); // y01をYへ回転させるクォータニオン
			float z0yang = vcg::math::ToDeg(vcg::Angle(z0 - y0, y1 - y0));
			Point3m y01_ = qua1.Rotate(y1-y0);
			Point3m z0_ = qua1.Rotate(z0-y0);
			Point3m z1_ = qua1.Rotate(z1-y0);
			Point3m z01 = z1_-z0_;
			vcg::Quaternionf qua2(vcg::Angle(z01,Z),z01^Z); // z01をZへ回転させるクォータニオン
			// 2つの変換を合成してから全体に適用(回転処理するので原点移動も前後でやっとく)
			Matrix44m M1,M2,M,tranM,tranMinv;
			tranM.SetTranslate(y0);
			tranMinv.SetTranslate(-y0);
			qua1.ToMatrix(M1);
			qua2.ToMatrix(M2);
			//M = tranM*M2*M1*tranMinv;
			M = M2*M1*tranMinv;

			Point3m a,b,c;
			a = M*y0;
			b = M*y1;
			c = M*z1;
			printP(a);
			printP(b);
			printP(c);
			int aaa = 100;
		}
	}

	return 0;
}
