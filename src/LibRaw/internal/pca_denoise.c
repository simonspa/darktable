#include "svd_arvo.c"

/**
 * converted from loony matlab code accompanying the following paper:
 * Lei Zhang, R. Lukac, X. Wu and D. Zhang, 
 * PCA-based Spatial Adaptive Denoising of CFA Images for Single-Sensor Digital Cameras,
 * IEEE Trans. on Image Processing, 2009.
 */
void CLASS pre_interpolate_pca_denoise()
{ // this is going to be so slow. :(
  ushort (*img)[4];
  const int pca_k = 34; // size of the training block
  const int pca_s = 6;  // size of the denoise block
  // const int pca_k2 = pca_k/2;
  const int N = (pca_k-pca_s)/2+1;
  const int D = pca_s*pca_s; // dimension of one vector
  const int L = N*N;  // number of training basis functions, 225 for the settings above
  int row, col, k, i, j, ii, jj, ind, ch;
  float vary, varn, c;
  const int var[4] = {32, 32, 32, 32}; // noise levels
  float Y[L][D];
  // TODO: init matrix D[pca_s,pca_s]: the noise pattern:
  //       entry-wise sigma^2 of respective underlying color channel

  img = (ushort (*)[4]) calloc (height*width, sizeof *image);
  merror (img, "pre_interpolate_pca_denoise()");
  memcpy(img,image,height*width*sizeof *image);

  for (row=0;row<height-pca_k;row+=2)
  {
    for (col=0;col<width-pca_k;col+=2)
    {
      // get pca_k by pca_k block starting at (col,row), we store the swizzled stuff in X
      float X[L][D];
      float varnx[D];

      ind = 0; // < D
      // for each dimension of one basis function
      for(j=0;j<pca_s;j++) for(i=0;i<pca_s;i++)
      { // store pca_s * pca_s block of cfa data (all 4 channels) at X[ind][..]
        k = 0; // < L
        ch = FC(row+j,col+i);
        // TODO: debug: see only one channel
        if(ch == 1)
          for(jj=0;jj<N;jj++) for(ii=0;ii<N;ii++) X[k++][ind] = img[width * (row+j+2*jj) + col + i+2*ii][ch];
        else
          for(jj=0;jj<N;jj++) for(ii=0;ii<N;ii++) X[k++][ind] = 0.0f;
        varnx[ind] = var[ch]; // remember channel and var for it.
        ind++;
      }

      // TODO: compare this to matlab input data:
      // for(k=0;k<D;k++) printf("X %d = %f\n", k, X[0][k]);

      // FIXME: is this needed at all?
      // TODO: find num most significant elements in X by simple heuristic on deviation from some center element
      const int num = L;

      // make X zero mean (subtract mean over first dimension X[.,L])
      float Xmean[D] = {0.0};
      for (k=0;k<D;k++) for(j=0;j<num;j++) Xmean[k] += X[j][k];
      for (k=0;k<D;k++) Xmean[k] *= (1.0/L);
      for (k=0;k<D;k++) for(j=0;j<num;j++) X[j][k]  -= Xmean[k];

      // for(k=0;k<D;k++) printf("X mean %d = %f\n", k, Xmean[k]);

      // for(k=0;k<D;k++) printf("X %d = %f\n", k, X[0][k]);

      // covariance matrix = X*X'/(L-1)
      float cov[D][D] = {{0.0}};
      for(k=0;k<D;k++) for(j=0;j<D;j++) for(i=0;i<num;i++) cov[k][j] += X[i][k] * X[i][j] * (1.0/(num-1.0));
      for(k=0;k<D;k++) cov[k][k] = fmaxf(0.0001, cov[k][k] - varnx[k]);

      /*for(j=0;j<D;j++)
      {
        for(k=0;k<D;k++) printf("%f ", cov[j][k]);
        printf("\n");
      }*/

      float eval[D];
      float vt[D*D];
      float *a = (float *)cov;
      // pca transform => Y = P*X (svd)
      // a = usv^t, a[n*m] is replaced by u[n*m], w[n] are the singular values as vect, vt[n*n] = v^t.
      svd(a, eval, vt, D, D);
      // printf("eval:\n");
      // for(k=0;k<D;k++) printf("%d -- %f\n", k, eval[k]);

      //  y = v^t * X
      for(k=0;k<L;k++)
      {
        for(i=0;i<D;i++)
        {
          Y[k][j] = 0.0f;
          for(j=0;j<D;j++) Y[k][i] += v[D*i+j]*X[k][j];
        }
      }
      // apply bayesian smoothing ( *= c with c = (vy - vn)/vy )
      for(k=0;k<D;k++)
      { // for all dimensions of a basis vector
        // get variance of y: E(Y^2) (zero mean)
        for(j=0;j<num;j++) vary += Y[j][k] * Y[j][k];
        vary *= 1.0/num;
        varn = 0.0f;
        // TODO: transform var[j] to pca space (including swizzling from L->num decimation)
        for(j=0;j<D;j++) varn += v[D*j+k] * varnx[j];
        c = fmaxf(0.0, vary - varn)/(vary + 0.0001f);
        for(j=0;j<num;j++) Y[j][k] *= c;
      }

      // TODO: backtransform B = P^t * Y + previously subtracted mean.
      for(k=0;k<L;k++)
      {
        for(i=0;i<D;i++)
        {
          X[k][j] = 0.0f;
          for(j=0;j<D;j++) X[k][i] += vt[D*j+i]*Y[k][j];
        }
      }

      // TODO: copy back the very center 2x2 values (1x all colors) of B
      // image[width*(j-1+pca_k2) + i-1+pca_k2][FC(j-1+pca_k2, i-1+pca_k2)] = 
    }
  }
  free(img);
}

