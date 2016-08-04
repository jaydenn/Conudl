#include <cmath>
#include <iostream>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_rng.h>
#include <iomanip>
#include "SMrate.h"
#include "BSMrate.h"
#ifndef PARAMETERSTRUCT_H
    #include "parameterStruct.h"
#endif
#ifndef DETECTORFUNCTIONS_H
	#include "detectorFunctions.h"
#endif

int SEED=0;

int generateBinnedData(paramList *pList, int detj, int b, int simSeed)
{

    double Er_min, Er_max;
    const gsl_rng_type * T;
    gsl_rng * r;
    gsl_rng_env_setup();
    T=gsl_rng_default;
    r=gsl_rng_alloc(T);
    gsl_rng_set(r, simSeed + SEED++);
    
    //total standard model, beyond SM and background rates
    double SM, BSM, BG;
    if(pList->BSM)
        BSM = pList->nuFluxNorm * intBSMrate( pList->detectors[detj].ErL, pList->detectors[detj].ErU, pList, detj, pList->signalNorm);
    else
        BSM = 0;
        
    SM = pList->nuFluxNorm * intSMrate( pList->detectors[detj].ErL, pList->detectors[detj].ErU, pList, detj);
    BG = pList->detectors[detj].BgNorm * b * intBgRate(pList->detectors[detj], pList->detectors[detj].ErL, pList->detectors[detj].ErU) ;     

    //std::cout << SM << " " << BG << " " << BSM << std::endl;

    //setup bins ~somewhat arbitrary choice of number of bins.. seems to work for exponential data
    if(SM + BSM + BG > 0)
        pList->detectors[detj].nbins = floor( sqrt(  pList->detectors[detj].exposure * ( SM + BSM + BG ) ) ) + 1;
    else
        pList->detectors[detj].nbins = 1;
        
    if (pList->detectors[detj].nbins > 80)
        pList->detectors[detj].nbins = 80;
    
    try
    {
        pList->detectors[detj].binnedData = new double[pList->detectors[detj].nbins];
    }
    catch (std::bad_alloc& ba)
    {
      std::cerr << "bad_alloc caught: " << ba.what() << std::endl << "you requested: " << pList->detectors[detj].nbins << " doubles" <<std::endl;
      return 1;
    }

    Er_min = pList->detectors[detj].ErL;    
    double logBinW = ( log10( pList->detectors[detj].ErU ) - log10 (pList->detectors[detj].ErL ) ) / ( (double) pList->detectors[detj].nbins);
    
    for(int i=0; i<pList->detectors[detj].nbins; i++)
    {
        
        if(pList->logBins==1)
            pList->detectors[detj].binW[i] = pow(10, log10(Er_min) + logBinW) - Er_min;
        else
            pList->detectors[detj].binW[i] = ( pList->detectors[detj].ErU - pList->detectors[detj].ErL ) / ( (double) pList->detectors[detj].nbins);
            
        Er_max = Er_min + pList->detectors[detj].binW[i];
        
        BSM = pList->nuFluxNorm * intBSMrate( Er_min, Er_max, pList, detj, pList->signalNorm);
        SM  = pList->nuFluxNorm * intSMrate( Er_min, Er_max, pList, detj); 
        BG  = pList->detectors[detj].BgNorm * b * intBgRate(pList->detectors[detj], Er_min, Er_max) ;
        
        if( pList->asimov == 1) 
            pList->detectors[detj].binnedData[i] = pList->detectors[detj].exposure *( SM + BSM + BG );
        else
            pList->detectors[detj].binnedData[i] = gsl_ran_poisson(r, pList->detectors[detj].exposure *( SM + BSM + BG ));                       
        
        pList->detectors[detj].nEvents += pList->detectors[detj].binnedData[i];
        
        Er_min = Er_max; //update lower bin limit
    }
   
    return 0;
     
}

