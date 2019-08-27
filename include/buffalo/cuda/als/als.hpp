#pragma once
#include <fstream>
#include <utility>

#include "json11.hpp"

struct cublasContext;

namespace cuda_als{

using namespace json11;

class CuALS{
public:
    CuALS();
    ~CuALS();

    bool init(std::string opt_path); 
    bool parse_option(std::string opt_path, Json& j);
    
    void set_placeholder(int64_t* lindptr, int64_t* rindptr, int batch_size);
    void initialize_model(
            float* P, int P_rows,
            float* Q, int Q_rows);
    void precompute(int axis);
    void synchronize(int axis, bool device_to_host);
    int get_vdim();
    std::pair<double, double> partial_update(int start_x, 
            int next_x,
            int64_t* indptr,
            int* keys,
            float* vals,
            int axis);
public:
    Json opt_;
    
    // left indptr, right indptr
    int64_t *lindptr_, *rindptr_;
    int* keys_; 
    float* vals_;
    int batch_size_;

    float *hostP_, *hostQ_, *devP_, *devQ_, *devFF_;
    float *hostLossNume_, *hostLossDeno_, *devLossNume_, *devLossDeno_;
    cublasContext* blas_handle_;
    int dim_, vdim_, num_cg_max_iters_, P_rows_, Q_rows_;
    float alpha_, reg_u_, reg_i_, cg_tolerance_, eps_;
    bool compute_loss_, adaptive_reg_;
    int devId_, mp_cnt_, block_cnt_;
    bool opt_setted_, initialized_, ph_setted_;
};

} // namespace cuda_als