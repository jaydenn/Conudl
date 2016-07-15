#include <iostream>
#include <fstream>
#include <iomanip>
#include "SMrate.h"
#include "BSMrate.h"
#include "detectorFunctions.h"
#ifndef PARAMETERSTRUCT_H
    #include "parameterStruct.h"
#endif

int calcRates(paramList *pList)
{
    double ErkeV;
    char filename[60];    
    std::ofstream outfile;
    char BSMname[15];
    
    switch(pList->BSM)
    {
        case 1:
        {
            sprintf(BSMname,"scalar");
            break;
        }
        case 2:
        {
            sprintf(BSMname,"pseudoS");
            break;
        }
        case 3:
        {
            sprintf(BSMname,"vector");
            break;
        }
        case 4:
        {
            sprintf(BSMname,"axial");
            break;
        }
    }
    
    //format output streams
    std::cout << setiosflags(std::ios::scientific) << std::setprecision(4);
    outfile   << setiosflags(std::ios::scientific) << std::setprecision(5);
    
    //output model
    int masskeV = (int)(pList->mMed*1e9);
    std::cout << "\nBSM rate for " << masskeV << " keV "<< BSMname << " mediator\n";
      
    for(int detj=0; detj < pList->ndet; detj++)
    {
        //open file
        if(pList->nucScat)
            sprintf(filename,"%s%s_%sRate%c_m%deV.dat", pList->root, pList->detectors[detj].name, BSMname,  'N', masskeV);
        else
            sprintf(filename,"%s%s_%sRate%c_m%deV.dat", pList->root, pList->detectors[detj].name, BSMname,  'E', masskeV);
        outfile.open(filename,std::ios::out);
        
        if(outfile==NULL)
        {
            std::cout << "output file could not be created" << std::endl;
            return 1;
        }
        
        std::cout << "------------------------\n";
        std::cout << detj+1 << ". " << pList->detectors[detj].name << std::endl;
        std::cout << "------------------------\n";
        std::cout << "  total rates: \n"; 
        std::cout << "     SM  = " << intSMrate(  pList->detectors[detj].ErL, pList->detectors[detj].ErU, pList, detj)        << " events/kg/day" << std::endl;
        std::cout << "     BG  = " << intBgRate(  pList->detectors[detj], pList->detectors[detj].ErL, pList->detectors[0].ErU) << " events/kg/day" << std::endl;
        std::cout << "     BSM = " << intBSMrate( pList->detectors[detj].ErL, pList->detectors[detj].ErU, pList, detj)        << " events/kg/day" << std::endl;
        
        std::cout << "  differential rates: \n"; 
        std::cout << "    Er (keV)        " << "SM dN/dE        " << "BG dN/dE";
        outfile   << "    Er (keV)         " << "SM dN/dE         " << "BG dN/dE";
        if(pList->BSM!=0)
        {
            std::cout << "        BSM dN/dE (events/kg/day/keV)" << std::endl;
            outfile   << "         BSM dN/dE (events/kg/day/keV)" << std::endl;
        }
        else
        {
            std::cout << " (events/kg/day/keV)" << std::endl;
            outfile   << " (events/kg/day/keV)" << std::endl;
        }
            
        for (int i=0; i<201; i+=1)
        {
            ErkeV = pList->detectors[detj].ErL + (double)i*(pList->detectors[detj].ErU-pList->detectors[detj].ErL)/200;
            std::cout << "    " << ErkeV << "      " << diffSMrate( ErkeV, pList, detj) << "      " << diffBgRate( pList->detectors[detj], ErkeV);
            outfile   << "    " << ErkeV << "      " << diffSMrate( ErkeV, pList, detj) << "      " << diffBgRate( pList->detectors[detj], ErkeV);
            
            if(pList->BSM!=0)
            {
                std::cout << "      " << diffBSMrate( ErkeV, pList, detj) << std::endl; 
                outfile   << "      " << diffBSMrate( ErkeV, pList, detj) << std::endl; 
            }
            else
            {
                std::cout << std::endl;
                outfile   << std::endl;
            }
        }
        
        outfile.close();
    }
    
}    