00001 
00023 /*
00024  * Copyright (c) 1991-2007 Kawahara Lab., Kyoto University
00025  * Copyright (c) 2000-2005 Shikano Lab., Nara Institute of Science and Technology
00026  * Copyright (c) 2005-2007 Julius project team, Nagoya Institute of Technology
00027  * All rights reserved
00028  */
00029 
00030 #include <sent/stddefs.h>
00031 #include <sent/mfcc.h>
00032 
00033 #ifdef MFCC_SINCOS_TABLE
00034 
00041 static void
00042 make_costbl_hamming(MFCCWork *w, int framesize)
00043 {
00044   int i;
00045   float a;
00046 
00047   w->costbl_hamming = (double *)mymalloc(sizeof(double) * framesize);
00048   a = 2.0 * PI / (framesize - 1);
00049   for(i=1;i<=framesize;i++) {
00050     /*costbl_hamming[i-1] = 0.54 - 0.46 * cos(2 * PI * (i - 1) / (float)(framesize - 1));*/
00051     w->costbl_hamming[i-1] = 0.54 - 0.46 * cos(a * (i - 1));
00052   }
00053   w->costbl_hamming_len = framesize;
00054 #ifdef MFCC_TABLE_DEBUG
00055   jlog("Stat: mfcc-core: generated Hamming cos table (%d bytes)\n",
00056        w->costbl_hamming_len * sizeof(double));
00057 #endif
00058 }
00059 
00066 static void
00067 make_fft_table(MFCCWork *w, int n)
00068 {
00069   int m;
00070   int me, me1;
00071   
00072   w->costbl_fft = (double *)mymalloc(sizeof(double) * n);
00073   w->sintbl_fft = (double *)mymalloc(sizeof(double) * n);
00074   for (m = 1; m <= n; m++) {
00075     me = 1 << m;
00076     me1 = me / 2;
00077     w->costbl_fft[m-1] =  cos(PI / me1);
00078     w->sintbl_fft[m-1] = -sin(PI / me1);
00079   }
00080   w->tbllen = n;
00081 #ifdef MFCC_TABLE_DEBUG
00082   jlog("Stat: mfcc-core: generated FFT sin/cos table (%d bytes)\n", w->tbllen * sizeof(double));
00083 #endif
00084 }
00085 
00093 static void
00094 make_costbl_makemfcc(MFCCWork *w, int fbank_num, int mfcc_dim)
00095 {
00096   int size;
00097   int i, j, k;
00098   float B, C;
00099 
00100   size = fbank_num * mfcc_dim;
00101   w->costbl_makemfcc = (double *)mymalloc(sizeof(double) * size);
00102 
00103   B = PI / fbank_num;
00104   k = 0;
00105   for(i=1;i<=mfcc_dim;i++) {
00106     C = i * B;
00107     for(j=1;j<=fbank_num;j++) {
00108       w->costbl_makemfcc[k] = cos(C * (j - 0.5));
00109       k++;
00110     }
00111   }
00112   w->costbl_makemfcc_len = size;
00113 #ifdef MFCC_TABLE_DEBUG
00114   jlog("Stat: mfcc-core: generated MakeMFCC cos table (%d bytes)\n",
00115        w->costbl_makemfcc_len * sizeof(double));
00116 #endif
00117 }
00118 
00126 static void
00127 make_sintbl_wcep(MFCCWork *w, int lifter, int mfcc_dim)
00128 {
00129   int i;
00130   float a, b;
00131 
00132   w->sintbl_wcep = (double *)mymalloc(sizeof(double) * mfcc_dim);
00133   a = PI / lifter;
00134   b = lifter / 2.0;
00135   for(i=0;i<mfcc_dim;i++) {
00136     w->sintbl_wcep[i] = 1.0 + b * sin((i+1) * a);
00137   }
00138   w->sintbl_wcep_len = mfcc_dim;
00139 #ifdef MFCC_TABLE_DEBUG
00140   jlog("Stat: mfcc-core: generated WeightCepstrum sin table (%d bytes)\n",
00141        w->sintbl_wcep_len * sizeof(double));
00142 #endif
00143 }
00144 
00145 #endif /* MFCC_SINCOS_TABLE */
00146 
00155 float Mel(int k, float fres)
00156 {
00157   return(1127 * log(1 + (k-1) * fres));
00158 }
00159 
00170 static boolean
00171 VTLN_recreate_fbank_cf(float *cf, Value *para, float mlo, float mhi, int maxChan)
00172 {
00173   int chan;
00174   float minf, maxf, cf_orig, cf_new;
00175   float scale, cu, cl, au, al;
00176 
00177   /* restore frequency range to non-Mel */
00178   minf = 700.0 * (exp(mlo / 1127.0) - 1.0);
00179   maxf = 700.0 * (exp(mhi / 1127.0) - 1.0);
00180 
00181   if (para->vtln_upper > maxf) {
00182     jlog("Error: VTLN upper cut-off greater than upper frequency bound: %.1f > %.1f\n", para->vtln_upper, maxf);
00183     return FALSE;
00184   }
00185   if (para->vtln_lower < minf) {
00186     jlog("Error: VTLN lower cut-off smaller than lower frequency bound: %.1f < %.1f\n", para->vtln_lower, minf);
00187     return FALSE;
00188   }
00189   
00190   /* prepare variables for warping */
00191   scale = 1.0 / para->vtln_alpha;
00192   cu = para->vtln_upper * 2 / ( 1 + scale);
00193   cl = para->vtln_lower * 2 / ( 1 + scale);
00194   au = (maxf - cu * scale) / (maxf - cu);
00195   al = (cl * scale - minf) / (cl - minf);
00196   
00197   for (chan = 1; chan <= maxChan; chan++) {
00198     /* get center frequency, restore to non-Mel */
00199     cf_orig = 700.0 * (exp(cf[chan] / 1127.0) - 1.0);
00200     /* do warping */
00201     if( cf_orig > cu ){
00202       cf_new = au * (cf_orig - cu) + scale * cu;
00203     } else if ( cf_orig < cl){
00204       cf_new = al * (cf_orig - minf) + minf;
00205     } else {
00206       cf_new = scale * cf_orig;
00207     }
00208     /* convert the new center frequency to Mel and store */
00209     cf[chan] = 1127.0 * log (1.0 + cf_new / 700.0);
00210   }
00211   return TRUE;
00212 }
00213 
00222 boolean
00223 InitFBank(MFCCWork *w, Value *para)
00224 {
00225   float mlo, mhi, ms, melk;
00226   int k, chan, maxChan, nv2;
00227 
00228   /* Calculate FFT size */
00229   w->fb.fftN = 2;  w->fb.n = 1;
00230   while(para->framesize > w->fb.fftN){
00231     w->fb.fftN *= 2; w->fb.n++;
00232   }
00233 
00234   nv2 = w->fb.fftN / 2;
00235   w->fb.fres = 1.0E7 / (para->smp_period * w->fb.fftN * 700.0);
00236   maxChan = para->fbank_num + 1;
00237   w->fb.klo = 2;   w->fb.khi = nv2;
00238   mlo = 0;      mhi = Mel(nv2 + 1, w->fb.fres);
00239 
00240   /* lo pass filter */
00241   if (para->lopass >= 0) {
00242     mlo = 1127*log(1+(float)para->lopass/700.0);
00243     w->fb.klo = ((float)para->lopass * para->smp_period * 1.0e-7 * w->fb.fftN) + 2.5;
00244     if (w->fb.klo<2) w->fb.klo = 2;
00245   }
00246   /* hi pass filter */
00247   if (para->hipass >= 0) {
00248     mhi = 1127*log(1+(float)para->hipass/700.0);
00249     w->fb.khi = ((float)para->hipass * para->smp_period * 1.0e-7 * w->fb.fftN) + 0.5;
00250     if (w->fb.khi>nv2) w->fb.khi = nv2;
00251   }
00252 
00253   /* Create vector of fbank centre frequencies */
00254   w->fb.cf = (float *)mymalloc((maxChan + 1) * sizeof(float));
00255   ms = mhi - mlo;
00256   for (chan = 1; chan <= maxChan; chan++) 
00257     w->fb.cf[chan] = ((float)chan / maxChan)*ms + mlo;
00258 
00259   if (para->vtln_alpha != 1.0) {
00260     /* Modify fbank center frequencies for VTLN */
00261     if (VTLN_recreate_fbank_cf(w->fb.cf, para, mlo, mhi, maxChan) == FALSE) {
00262       return FALSE;
00263     }
00264   }
00265 
00266   /* Create loChan map, loChan[fftindex] -> lower channel index */
00267   w->fb.loChan = (short *)mymalloc((nv2 + 1) * sizeof(short));
00268   for(k = 1, chan = 1; k <= nv2; k++){
00269     if (k < w->fb.klo || k > w->fb.khi) w->fb.loChan[k] = -1;
00270     else {
00271       melk = Mel(k, w->fb.fres);
00272       while (w->fb.cf[chan] < melk && chan <= maxChan) ++chan;
00273       w->fb.loChan[k] = chan - 1;
00274     }
00275   }
00276 
00277   /* Create vector of lower channel weights */   
00278   w->fb.loWt = (float *)mymalloc((nv2 + 1) * sizeof(float));
00279   for(k = 1; k <= nv2; k++) {
00280     chan = w->fb.loChan[k];
00281     if (k < w->fb.klo || k > w->fb.khi) w->fb.loWt[k] = 0.0;
00282     else {
00283       if (chan > 0) 
00284         w->fb.loWt[k] = (w->fb.cf[chan + 1] - Mel(k, w->fb.fres)) / (w->fb.cf[chan + 1] - w->fb.cf[chan]);
00285       else
00286         w->fb.loWt[k] = (w->fb.cf[1] - Mel(k, w->fb.fres)) / (w->fb.cf[1] - mlo);
00287     }
00288   }
00289   
00290   /* Create workspace for fft */
00291   w->fb.Re = (float *)mymalloc((w->fb.fftN + 1) * sizeof(float));
00292   w->fb.Im = (float *)mymalloc((w->fb.fftN + 1) * sizeof(float));
00293 
00294   w->sqrt2var = sqrt(2.0 / para->fbank_num);
00295 
00296   return TRUE;
00297 }
00298 
00304 void
00305 FreeFBank(FBankInfo *fb)
00306 {
00307   free(fb->cf);
00308   free(fb->loChan);
00309   free(fb->loWt);
00310   free(fb->Re);
00311   free(fb->Im);
00312 }
00313 
00321 void
00322 ZMeanFrame(float *wave, int framesize)
00323 {                  
00324   int i;
00325   float mean;
00326 
00327   mean = 0.0;
00328   for(i = 1; i <= framesize; i++) mean += wave[i];
00329   mean /= framesize;
00330   for(i = 1; i <= framesize; i++) wave[i] -= mean;
00331 }
00332 
00341 float CalcLogRawE(float *wave, int framesize)
00342 {                  
00343   int i;
00344   double raw_E = 0.0;
00345   float energy;
00346 
00347   for(i = 1; i <= framesize; i++)
00348     raw_E += wave[i] * wave[i];
00349   energy = (float)log(raw_E);
00350 
00351   return(energy);
00352 }
00353 
00361 void PreEmphasise (float *wave, int framesize, float preEmph)
00362 {
00363   int i;
00364    
00365   for(i = framesize; i >= 2; i--)
00366     wave[i] -= wave[i - 1] * preEmph;
00367   wave[1] *= 1.0 - preEmph;  
00368 }
00369 
00377 void Hamming(float *wave, int framesize, MFCCWork *w)
00378 {
00379   int i;
00380 #ifdef MFCC_SINCOS_TABLE
00381   for(i = 1; i <= framesize; i++)
00382     wave[i] *= w->costbl_hamming[i-1];
00383 #else
00384   float a;
00385   a = 2 * PI / (framesize - 1);
00386   for(i = 1; i <= framesize; i++)
00387     wave[i] *= 0.54 - 0.46 * cos(a * (i - 1));
00388 #endif
00389 }
00390 
00399 void FFT(float *xRe, float *xIm, int p, MFCCWork *w)
00400 {
00401   int i, ip, j, k, m, me, me1, n, nv2;
00402   double uRe, uIm, vRe, vIm, wRe, wIm, tRe, tIm;
00403   
00404   n = 1<<p;
00405   nv2 = n / 2;
00406   
00407   j = 0;
00408   for(i = 0; i < n-1; i++){
00409     if(j > i){
00410       tRe = xRe[j];      tIm = xIm[j];
00411       xRe[j] = xRe[i];   xIm[j] = xIm[i];
00412       xRe[i] = tRe;      xIm[i] = tIm;
00413     }
00414     k = nv2;
00415     while(j >= k){
00416       j -= k;      k /= 2;
00417     }
00418     j += k;
00419   }
00420 
00421   for(m = 1; m <= p; m++){
00422     me = 1<<m;                me1 = me / 2;
00423     uRe = 1.0;                uIm = 0.0;
00424 #ifdef MFCC_SINCOS_TABLE
00425     wRe = w->costbl_fft[m-1];    wIm = w->sintbl_fft[m-1];
00426 #else
00427     wRe = cos(PI / me1);      wIm = -sin(PI / me1);
00428 #endif
00429     for(j = 0; j < me1; j++){
00430       for(i = j; i < n; i += me){
00431         ip = i + me1;
00432         tRe = xRe[ip] * uRe - xIm[ip] * uIm;
00433         tIm = xRe[ip] * uIm + xIm[ip] * uRe;
00434         xRe[ip] = xRe[i] - tRe;   xIm[ip] = xIm[i] - tIm;
00435         xRe[i] += tRe;            xIm[i] += tIm;
00436       }
00437       vRe = uRe * wRe - uIm * wIm;   vIm = uRe * wIm + uIm * wRe;
00438       uRe = vRe;                     uIm = vIm;
00439     }
00440   }
00441 }
00442 
00443 
00451 void
00452 MakeFBank(float *wave, MFCCWork *w, Value *para)
00453 {
00454   int k, bin, i;
00455   double Re, Im, A, P, NP, H, temp;
00456 
00457   for(k = 1; k <= para->framesize; k++){
00458     w->fb.Re[k - 1] = wave[k];  w->fb.Im[k - 1] = 0.0;  /* copy to workspace */
00459   }
00460   for(k = para->framesize + 1; k <= w->fb.fftN; k++){
00461     w->fb.Re[k - 1] = 0.0;      w->fb.Im[k - 1] = 0.0;  /* pad with zeroes */
00462   }
00463   
00464   /* Take FFT */
00465   FFT(w->fb.Re, w->fb.Im, w->fb.n, w);
00466 
00467   if (w->ssbuf != NULL) {
00468     /* Spectral Subtraction */
00469     for(k = 1; k <= w->fb.fftN; k++){
00470       Re = w->fb.Re[k - 1];  Im = w->fb.Im[k - 1];
00471       P = sqrt(Re * Re + Im * Im);
00472       NP = w->ssbuf[k - 1];
00473       if((P * P -  w->ss_alpha * NP * NP) < 0){
00474         H = w->ss_floor;
00475       }else{
00476         H = sqrt(P * P - w->ss_alpha * NP * NP) / P;
00477       }
00478       w->fb.Re[k - 1] = H * Re;
00479       w->fb.Im[k - 1] = H * Im;
00480     }
00481   }
00482 
00483   /* Fill filterbank channels */ 
00484   for(i = 1; i <= para->fbank_num; i++)
00485     w->fbank[i] = 0.0;
00486   
00487   if (para->usepower) {
00488     for(k = w->fb.klo; k <= w->fb.khi; k++){
00489       Re = w->fb.Re[k-1]; Im = w->fb.Im[k-1];
00490       A = Re * Re + Im * Im;
00491       bin = w->fb.loChan[k];
00492       Re = w->fb.loWt[k] * A;
00493       if(bin > 0) w->fbank[bin] += Re;
00494       if(bin < para->fbank_num) w->fbank[bin + 1] += A - Re;
00495     }
00496   } else {
00497     for(k = w->fb.klo; k <= w->fb.khi; k++){
00498       Re = w->fb.Re[k-1]; Im = w->fb.Im[k-1];
00499       A = sqrt(Re * Re + Im * Im);
00500       bin = w->fb.loChan[k];
00501       Re = w->fb.loWt[k] * A;
00502       if(bin > 0) w->fbank[bin] += Re;
00503       if(bin < para->fbank_num) w->fbank[bin + 1] += A - Re;
00504     }
00505   }
00506 
00507   /* Take logs */
00508   for(bin = 1; bin <= para->fbank_num; bin++){ 
00509     temp = w->fbank[bin];
00510     if(temp < 1.0) temp = 1.0;
00511     w->fbank[bin] = log(temp);  
00512   }
00513 }
00514 
00523 float CalcC0(MFCCWork *w, Value *para)
00524 {
00525   int i; 
00526   float S;
00527   
00528   S = 0.0;
00529   for(i = 1; i <= para->fbank_num; i++)
00530     S += w->fbank[i];
00531   return S * w->sqrt2var;
00532 }
00533 
00541 void MakeMFCC(float *mfcc, Value *para, MFCCWork *w)
00542 {
00543 #ifdef MFCC_SINCOS_TABLE
00544   int i, j, k;
00545   k = 0;
00546   /* Take DCT */
00547   for(i = 0; i < para->mfcc_dim; i++){
00548     mfcc[i] = 0.0;
00549     for(j = 1; j <= para->fbank_num; j++)
00550       mfcc[i] += w->fbank[j] * w->costbl_makemfcc[k++];
00551     mfcc[i] *= w->sqrt2var;
00552   }
00553 #else
00554   int i, j;
00555   float B, C;
00556   
00557   B = PI / para->fbank_num;
00558   /* Take DCT */
00559   for(i = 1; i <= para->mfcc_dim; i++){
00560     mfcc[i - 1] = 0.0;
00561     C = i * B;
00562     for(j = 1; j <= para->fbank_num; j++)
00563       mfcc[i - 1] += w->fbank[j] * cos(C * (j - 0.5));
00564     mfcc[i - 1] *= w->sqrt2var;     
00565   }
00566 #endif
00567 }
00568 
00576 void WeightCepstrum (float *mfcc, Value *para, MFCCWork *w)
00577 {
00578 #ifdef MFCC_SINCOS_TABLE
00579   int i;
00580   for(i=0;i<para->mfcc_dim;i++) {
00581     mfcc[i] *= w->sintbl_wcep[i];
00582   }
00583 #else
00584   int i;
00585   float a, b, *cepWin;
00586   
00587   cepWin = (float *)mymalloc(para->mfcc_dim * sizeof(float));
00588   a = PI / para->lifter;
00589   b = para->lifter / 2.0;
00590   
00591   for(i = 0; i < para->mfcc_dim; i++){
00592     cepWin[i] = 1.0 + b * sin((i + 1) * a);
00593     mfcc[i] *= cepWin[i];
00594   }
00595   
00596   free(cepWin);
00597 #endif
00598 }
00599 
00600 /************************************************************************/
00601 /************************************************************************/
00602 /************************************************************************/
00603 /************************************************************************/
00604 /************************************************************************/
00613 MFCCWork *
00614 WMP_work_new(Value *para)
00615 {
00616   MFCCWork *w;
00617 
00618   /* newly allocated area should be cleared */
00619   w = (MFCCWork *)mymalloc(sizeof(MFCCWork));
00620   memset(w, 0, sizeof(MFCCWork));
00621 
00622   /* set filterbank information */
00623   if (InitFBank(w, para) == FALSE) return NULL;
00624 
00625 #ifdef MFCC_SINCOS_TABLE
00626   /* prepare tables */
00627   make_costbl_hamming(w, para->framesize);
00628   make_fft_table(w, w->fb.n);
00629   if (para->mfcc_dim >= 0) {
00630     make_costbl_makemfcc(w, para->fbank_num, para->mfcc_dim);
00631     make_sintbl_wcep(w, para->lifter, para->mfcc_dim);
00632   }
00633 #endif
00634 
00635   /* prepare some buffers */
00636   w->fbank = (double *)mymalloc((para->fbank_num+1)*sizeof(double));
00637   w->bf = (float *)mymalloc(w->fb.fftN * sizeof(float));
00638   w->bflen = w->fb.fftN;
00639 
00640   return w;
00641 }
00642 
00651 void
00652 WMP_calc(MFCCWork *w, float *mfcc, Value *para)
00653 {
00654   float energy = 0.0;
00655   float c0 = 0.0;
00656   int p;
00657 
00658   if (para->zmeanframe) {
00659     ZMeanFrame(w->bf, para->framesize);
00660   }
00661 
00662   if (para->energy && para->raw_e) {
00663     /* calculate log raw energy */
00664     energy = CalcLogRawE(w->bf, para->framesize);
00665   }
00666   /* pre-emphasize */
00667   PreEmphasise(w->bf, para->framesize, para->preEmph);
00668   /* hamming window */
00669   Hamming(w->bf, para->framesize, w);
00670   if (para->energy && ! para->raw_e) {
00671     /* calculate log energy */
00672     energy = CalcLogRawE(w->bf, para->framesize);
00673   }
00674   /* filterbank */
00675   MakeFBank(w->bf, w, para);
00676   /* 0'th cepstral parameter */
00677   if (para->c0) c0 = CalcC0(w, para);
00678   /* MFCC */
00679   MakeMFCC(mfcc, para, w);
00680   /* weight cepstrum */
00681   WeightCepstrum(mfcc, para, w);
00682   /* set energy to mfcc */
00683   p = para->mfcc_dim;
00684   if (para->c0) mfcc[p++] = c0;
00685   if (para->energy) mfcc[p++] = energy;
00686 }
00687 
00693 void
00694 WMP_free(MFCCWork *w)
00695 {
00696   if (w->fbank) {
00697     FreeFBank(&(w->fb));
00698     free(w->fbank);
00699     free(w->bf);
00700     w->fbank = NULL;
00701     w->bf = NULL;
00702   }
00703 #ifdef MFCC_SINCOS_TABLE
00704   if (w->costbl_hamming) {
00705     free(w->costbl_hamming);
00706     w->costbl_hamming = NULL;
00707   }
00708   if (w->costbl_fft) {
00709     free(w->costbl_fft);
00710     w->costbl_fft = NULL;
00711   }
00712   if (w->sintbl_fft) {
00713     free(w->sintbl_fft);
00714     w->sintbl_fft = NULL;
00715   }
00716   if (w->costbl_makemfcc) {
00717     free(w->costbl_makemfcc);
00718     w->costbl_makemfcc = NULL;
00719   }
00720   if (w->sintbl_wcep) {
00721     free(w->sintbl_wcep);
00722     w->sintbl_wcep = NULL;
00723   }
00724 #endif
00725   free(w);
00726 }
00727 
