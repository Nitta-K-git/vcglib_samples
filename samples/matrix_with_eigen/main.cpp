#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/complex.h>
#include <common/ml_mesh_type.h>

using namespace vcg;
using namespace std;

template <typename T>
using RMatrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
typedef RMatrix<MESHLAB_SCALAR> RMatrixm;
using RefRMatrix = Eigen::Ref<const RMatrixm>;
using EVector = Eigen::Vector3f;

int main(){
	Matrix44m M;
	RMatrixm eigenM = RMatrixm::Identity(4,4); // set matrix size
	M.ToEigenMatrix<RMatrixm>(eigenM);
	cout << eigenM << endl;

	RMatrix<float> em = RMatrix<float>::Ones(4,4);;
	M.FromEigenMatrix<Eigen::Ref<const RMatrixm>>(em);
	return 1;
}

