#ifndef weno5js_cpp
#define weno5js_cpp
#include "weno5z.hpp"
#include "schemes.hpp"
#include <cmath>
nuc3d::weno5z::weno5z():
ss(1.0e-40),
p(2)
{
    
}

nuc3d::weno5z::~weno5z()
{
    
}

void nuc3d::weno5z::interpolationInner(const Field & fieldIN,
                                        const int uw,//uw=(1,-1)
                                        Field & fieldOUT,
                                        const int tilesize)
{
    switch(uw)
    {
        case 1:
            weno5zp(fieldIN, fieldOUT, tilesize);
            break;
        case -1:
            weno5zn(fieldIN, fieldOUT, tilesize);
            break;
        default:
            std::cout<<"weno error: no such direction"<<std::endl;
            exit(-1);
    }
    
}

void nuc3d::weno5z::weno5zp(const Field & fieldIN,
                              Field & fieldOUT,
                              const int tilesize)
{
    
    double *pIn=fieldIN.getDataPtr();
    double *pOut=fieldOUT.getDataPtr();
        
    int nx=fieldIN.getSizeX();
    int ny=fieldIN.getSizeY();
    int nz=fieldIN.getSizeZ();
    
    int nx0=fieldOUT.getSizeX();
    int ny0=fieldOUT.getSizeY();
    int nz0=fieldOUT.getSizeZ();
    
    int ibeg=(tilesize-1);
    int iend=nx-tilesize;
    int jbeg=0;
    int jend=ny;
    int kbeg=0;
    int kend=nz;
    
    for(int k=kbeg;k<kend;k++)
    {
        double *pData_in_k=pIn+nx*ny*k;
        double *pData_out_k=pOut+nx0*ny0*k;
        
        for(int j=jbeg;j<jend;j++)
        {
            double *pData_in=pData_in_k+nx*j;
            double *pData_out=pData_out_k+nx0*j;

            for(int i=ibeg;i<iend;i++)
            {       
                
                double is0= coeff_weno5_gamma0*std::pow((pData_in[i-2]-2.0*pData_in[i-1]+pData_in[i]),p)
                +coeff_weno5_gamma1*std::pow((pData_in[i-2]-4.0*pData_in[i-1]+3.0*pData_in[i]),p);
                
                double is1= coeff_weno5_gamma0*std::pow((pData_in[i-1]-2.0*pData_in[i]+pData_in[i+1]),p)
                +coeff_weno5_gamma1*std::pow((pData_in[i-1]-pData_in[i+1]),p);
                
                double is2= coeff_weno5_gamma0*std::pow((pData_in[i]-2.0*pData_in[i+1]+pData_in[i+1]),p)
                +coeff_weno5_gamma1*std::pow((3.0*pData_in[i]-4.0*pData_in[i+1]+pData_in[i+2]),p);
                
                
                double q30= coeff_weno5_alpha[0][0]*pData_in[i-2]
                +coeff_weno5_alpha[0][1]*pData_in[i-1]
                +coeff_weno5_alpha[0][2]*pData_in[i];
                
                double q31= coeff_weno5_alpha[1][0]*pData_in[i-1]
                +coeff_weno5_alpha[1][1]*pData_in[i]
                +coeff_weno5_alpha[1][2]*pData_in[i+1];
                
                double q32= coeff_weno5_alpha[2][0]*pData_in[i]
                +coeff_weno5_alpha[2][1]*pData_in[i+1]
                +coeff_weno5_alpha[2][2]*pData_in[i+2];
                
                double tau=std::fabs(is2-is0);
                
                double alpha0=coeff_weno5_c[0]*(1.0+std::pow(tau/(is0+ss),p));
                double alpha1=coeff_weno5_c[1]*(1.0+std::pow(tau/(is1+ss),p));
                double alpha2=coeff_weno5_c[2]*(1.0+std::pow(tau/(is2+ss),p));
                
                
                double alphaSum=alpha0+alpha1+alpha2;
                
                double omega0=alpha0/alphaSum;
                double omega1=alpha1/alphaSum;
                double omega2=alpha2/alphaSum;
                
                pData_out[i+1]=omega0*q30+omega1*q31+omega2*q32;
                
            }
        }
    }
}

void nuc3d::weno5z::weno5zn(const Field & fieldIN,
                              Field & fieldOUT,
                              const int tilesize)
{
    
    double *pIn=fieldIN.getDataPtr();
    double *pOut=fieldOUT.getDataPtr();
        
    int nx=fieldIN.getSizeX();
    int ny=fieldIN.getSizeY();
    int nz=fieldIN.getSizeZ();
    
    int nx0=fieldOUT.getSizeX();
    int ny0=fieldOUT.getSizeY();
    int nz0=fieldOUT.getSizeZ();
    
    int ibeg=(tilesize-1);
    int iend=nx-tilesize;
    int jbeg=0;
    int jend=ny;
    int kbeg=0;
    int kend=nz;
    
    
    for(int k=kbeg;k<kend;k++)
    {
        double *pData_in_k=pIn+nx*ny*k;
        double *pData_out_k=pOut+nx0*ny0*k;
        
        for(int j=jbeg;j<jend;j++)
        {
            double *pData_in=pData_in_k+nx*j;
            double *pData_out=pData_out_k+nx0*j;

            for(int i=ibeg;i<iend;i++)
            {       
                
                double is0= coeff_weno5_gamma0*std::pow((pData_in[i+3]-2.0*pData_in[i+2]+pData_in[i+1]),p)
                +coeff_weno5_gamma1*std::pow((pData_in[i+3]-4.0*pData_in[i+2]+3.0*pData_in[i+1]),p);
                
                double is1= coeff_weno5_gamma0*std::pow((pData_in[i+2]-2.0*pData_in[i+1]+pData_in[i]),p)
                +coeff_weno5_gamma1*std::pow((pData_in[i+2]-pData_in[i]),p);
                
                double is2= coeff_weno5_gamma0*std::pow((pData_in[i+1]-2.0*pData_in[i]+pData_in[i-1]),p)
                +coeff_weno5_gamma1*std::pow((3.0*pData_in[i+1]-4.0*pData_in[i]+pData_in[i-1]),p);
                
                
                double q30= coeff_weno5_alpha[0][0]*pData_in[i+3]
                +coeff_weno5_alpha[0][1]*pData_in[i+2]
                +coeff_weno5_alpha[0][2]*pData_in[i+1];
                
                double q31= coeff_weno5_alpha[1][0]*pData_in[i+2]
                +coeff_weno5_alpha[1][1]*pData_in[i+1]
                +coeff_weno5_alpha[1][2]*pData_in[i];
                
                double q32= coeff_weno5_alpha[2][0]*pData_in[i+1]
                +coeff_weno5_alpha[2][1]*pData_in[i]
                +coeff_weno5_alpha[2][2]*pData_in[i-1];
                
                double tau=std::fabs(is2-is0);
                
                double alpha0=coeff_weno5_c[0]*(1.0+std::pow(tau/(is0+ss),p));
                double alpha1=coeff_weno5_c[1]*(1.0+std::pow(tau/(is1+ss),p));
                double alpha2=coeff_weno5_c[2]*(1.0+std::pow(tau/(is2+ss),p));
                
                
                double alphaSum=alpha0+alpha1+alpha2;
                
                double omega0=alpha0/alphaSum;
                double omega1=alpha1/alphaSum;
                double omega2=alpha2/alphaSum;
                
                pData_out[i+1]=omega0*q30+omega1*q31+omega2*q32;
                
            }
        }
    }
}

void nuc3d::weno5z::interpolationBoundaryL(const Field & fieldIN,
                                            const Field & boundaryL,
                                            const int uw,//uw=(1,-1)
                                            Field & fieldOUT,
                                            const int tilesize)
{
    switch(uw)
    {
        case 1:
            weno5zpBL(fieldIN,boundaryL, fieldOUT, tilesize);
            break;
        case -1:
            weno5znBL(fieldIN,boundaryL, fieldOUT, tilesize);
            break;
        default:
            std::cout<<"weno error: no such direction"<<std::endl;
            exit(-1);
    }
    
}

void nuc3d::weno5z::weno5zpBL(const Field & fieldIN,
                                const Field & boundaryL,
                                Field & fieldOUT,
                                const int tilesize)
{
    double *pIn=fieldIN.getDataPtr();
    double *pOut=fieldOUT.getDataPtr();
    double *pBND=boundaryL.getDataPtr();
    
    double omega0,omega1,omega2;
    double alpha0,alpha1,alpha2,alphaSum;
    double is0,is1,is2;
    double q30,q31,q32;
    
    int nx=fieldIN.getSizeX();
    int ny=fieldIN.getSizeY();
    int nz=fieldIN.getSizeZ();
    
    int nx0=fieldOUT.getSizeX();
    int ny0=fieldOUT.getSizeY();
    int nz0=fieldOUT.getSizeZ();
    
    int nxBND=boundaryL.getSizeX();
    int nyBND=boundaryL.getSizeY();
    int nzBND=boundaryL.getSizeZ();
    
    int ibeg=0;
    int iend=tilesize;
    int jbeg=0;
    int jend=ny;
    int kbeg=0;
    int kend=nz;
    
    double f[5];
    
    
    for(int k=kbeg;k<kend;k++)
    {
        for(int j=jbeg;j<jend;j++)
        {
            for(int i=ibeg;i<iend;i++)
            {
                int idx_rf=nx0*ny0*k+nx0*j+i;
                for(int z=-2;z<=2;z++)
                {
                    if((i+z-1)<0)
                    {
                        int idx_BND=nxBND*nyBND*k+nxBND*j+(i+z-1+nxBND);
                        
                        f[2+z]=pBND[idx_BND];
                    }
                    else
                    {
                        int idx_f=nx*ny*k+nx*j+i+z-1;
                        f[2+z]=pIn[idx_f];
                    }
                }
                
                
                is0= coeff_weno5_gamma0*std::pow((    f[0]-2.0*f[1]+    f[2]),p)
                +coeff_weno5_gamma1*std::pow((    f[0]-4.0*f[1]+3.0*f[2]),p);
                
                is1= coeff_weno5_gamma0*std::pow((    f[1]-2.0*f[2]+    f[3]),p)
                +coeff_weno5_gamma1*std::pow((    f[1]-             f[3]),p);
                
                is2= coeff_weno5_gamma0*std::pow((    f[2]-2.0*f[3]+    f[4]),p)
                +coeff_weno5_gamma1*std::pow((3.0*f[2]-4.0*f[3]+    f[4]),p);
                
                
                q30= coeff_weno5_alpha[0][0]*f[0]
                +coeff_weno5_alpha[0][1]*f[1]
                +coeff_weno5_alpha[0][2]*f[2];
                
                q31= coeff_weno5_alpha[1][0]*f[1]
                +coeff_weno5_alpha[1][1]*f[2]
                +coeff_weno5_alpha[1][2]*f[3];
                
                q32= coeff_weno5_alpha[2][0]*f[2]
                +coeff_weno5_alpha[2][1]*f[3]
                +coeff_weno5_alpha[2][2]*f[4];
                
                double tau=std::fabs(is2-is0);
                
                alpha0=coeff_weno5_c[0]*(1.0+std::pow(tau/(is0+ss),p));
                alpha1=coeff_weno5_c[1]*(1.0+std::pow(tau/(is1+ss),p));
                alpha2=coeff_weno5_c[2]*(1.0+std::pow(tau/(is2+ss),p));
               
                
                alphaSum=alpha0+alpha1+alpha2;
                
                omega0=alpha0/alphaSum;
                omega1=alpha1/alphaSum;
                omega2=alpha2/alphaSum;
                
                pOut[idx_rf]=omega0*q30+omega1*q31+omega2*q32;
            }
        }
    }
}

void nuc3d::weno5z::weno5znBL(const Field & fieldIN,
                                const Field & boundaryL,
                                Field & fieldOUT,
                                const int tilesize)
{
    double *pIn=fieldIN.getDataPtr();
    double *pOut=fieldOUT.getDataPtr();
    double *pBND=boundaryL.getDataPtr();
    
    double omega0,omega1,omega2;
    double alpha0,alpha1,alpha2,alphaSum;
    double is0,is1,is2;
    double q30,q31,q32;
    
    int nx=fieldIN.getSizeX();
    int ny=fieldIN.getSizeY();
    int nz=fieldIN.getSizeZ();
    
    int nx0=fieldOUT.getSizeX();
    int ny0=fieldOUT.getSizeY();
    int nz0=fieldOUT.getSizeZ();
    
    int nxBND=boundaryL.getSizeX();
    int nyBND=boundaryL.getSizeY();
    int nzBND=boundaryL.getSizeZ();
    
    int ibeg=0;
    int iend=tilesize;
    int jbeg=0;
    int jend=ny;
    int kbeg=0;
    int kend=nz;
    
    double f[5];
    
    for(int k=kbeg;k<kend;k++)
    {
        for(int j=jbeg;j<jend;j++)
        {
            for(int i=ibeg;i<iend;i++)
            {
                int idx_rf=nx0*ny0*k+nx0*j+i;
                
                for(int z=-2;z<=2;z++)
                {
                    if((i-z)<0)
                    {
                        int idx_BND=nxBND*nyBND*k+nxBND*j+(i-z+nxBND);
                        
                        f[2+z]=pBND[idx_BND];
                    }
                    else
                    {
                        int idx_f=nx*ny*k+nx*j+i-z;
                        f[2+z]=pIn[idx_f];
                    }
                }
                
                
                is0= coeff_weno5_gamma0*std::pow((    f[0]-2.0*f[1]+    f[2]),p)
                +coeff_weno5_gamma1*std::pow((    f[0]-4.0*f[1]+3.0*f[2]),p);
                
                is1= coeff_weno5_gamma0*std::pow((    f[1]-2.0*f[2]+    f[3]),p)
                +coeff_weno5_gamma1*std::pow((    f[1]-             f[3]),p);
                
                is2= coeff_weno5_gamma0*std::pow((    f[2]-2.0*f[3]+    f[4]),p)
                +coeff_weno5_gamma1*std::pow((3.0*f[2]-4.0*f[3]+    f[4]),p);
                
                
                q30= coeff_weno5_alpha[0][0]*f[0]
                +coeff_weno5_alpha[0][1]*f[1]
                +coeff_weno5_alpha[0][2]*f[2];
                
                q31= coeff_weno5_alpha[1][0]*f[1]
                +coeff_weno5_alpha[1][1]*f[2]
                +coeff_weno5_alpha[1][2]*f[3];
                
                q32= coeff_weno5_alpha[2][0]*f[2]
                +coeff_weno5_alpha[2][1]*f[3]
                +coeff_weno5_alpha[2][2]*f[4];
                
                double tau=std::fabs(is2-is0);
                
                alpha0=coeff_weno5_c[0]*(1.0+std::pow(tau/(is0+ss),p));
                alpha1=coeff_weno5_c[1]*(1.0+std::pow(tau/(is1+ss),p));
                alpha2=coeff_weno5_c[2]*(1.0+std::pow(tau/(is2+ss),p));
                
                
                alphaSum=alpha0+alpha1+alpha2;
                
                omega0=alpha0/alphaSum;
                omega1=alpha1/alphaSum;
                omega2=alpha2/alphaSum;
                
                pOut[idx_rf]=omega0*q30+omega1*q31+omega2*q32;
            }
        }
    }
}

void nuc3d::weno5z::interpolationBoundaryR(const Field & fieldIN,
                                            const Field & boundaryR,
                                            const int uw,//uw=(1,-1)
                                            Field & fieldOUT,
                                            const int tilesize)
{
    switch(uw)
    {
        case 1:
            weno5zpBR(fieldIN,boundaryR, fieldOUT, tilesize);
            break;
        case -1:
            weno5znBR(fieldIN,boundaryR, fieldOUT, tilesize);
            break;
        default:
            std::cout<<"weno error: no such direction"<<std::endl;
            exit(-1);
    }
    
}


void nuc3d::weno5z::weno5zpBR(const Field & fieldIN,
                                const Field & boundaryR,
                                Field & fieldOUT,
                                const int tilesize)
{
    double *pIn=fieldIN.getDataPtr();
    double *pOut=fieldOUT.getDataPtr();
    double *pBND=boundaryR.getDataPtr();
    
    double omega0,omega1,omega2;
    double alpha0,alpha1,alpha2,alphaSum;
    double is0,is1,is2;
    double q30,q31,q32;
    
    int nx=fieldIN.getSizeX();
    int ny=fieldIN.getSizeY();
    int nz=fieldIN.getSizeZ();
    
    int nx0=fieldOUT.getSizeX();
    int ny0=fieldOUT.getSizeY();
    int nz0=fieldOUT.getSizeZ();
    
    int nxBND=boundaryR.getSizeX();
    int nyBND=boundaryR.getSizeY();
    int nzBND=boundaryR.getSizeZ();
    
    int ibeg=nx-tilesize;
    int iend=nx;
    int jbeg=0;
    int jend=ny;
    int kbeg=0;
    int kend=nz;
    
    double f[5];
    
    
    for(int k=kbeg;k<kend;k++)
    {
        for(int j=jbeg;j<jend;j++)
        {
            for(int i=ibeg;i<iend;i++)
            {
                int idx_rf=nx0*ny0*k+nx0*j+i+1;
                for(int z=-2;z<=2;z++)
                {
                    if((i+z)>=nx)
                    {
                        int idx_BND=nxBND*nyBND*k+nxBND*j+i+z-nx;
                        
                        f[2+z]=pBND[idx_BND];
                    }
                    else
                    {
                        int idx_f=nx*ny*k+nx*j+i+z;
                        f[2+z]=pIn[idx_f];
                    }
                }
                
                
                is0= coeff_weno5_gamma0*std::pow((    f[0]-2.0*f[1]+    f[2]),p)
                +coeff_weno5_gamma1*std::pow((    f[0]-4.0*f[1]+3.0*f[2]),p);
                
                is1= coeff_weno5_gamma0*std::pow((    f[1]-2.0*f[2]+    f[3]),p)
                +coeff_weno5_gamma1*std::pow((    f[1]-             f[3]),p);
                
                is2= coeff_weno5_gamma0*std::pow((    f[2]-2.0*f[3]+    f[4]),p)
                +coeff_weno5_gamma1*std::pow((3.0*f[2]-4.0*f[3]+    f[4]),p);
                
                
                q30= coeff_weno5_alpha[0][0]*f[0]
                +coeff_weno5_alpha[0][1]*f[1]
                +coeff_weno5_alpha[0][2]*f[2];
                
                q31= coeff_weno5_alpha[1][0]*f[1]
                +coeff_weno5_alpha[1][1]*f[2]
                +coeff_weno5_alpha[1][2]*f[3];
                
                q32= coeff_weno5_alpha[2][0]*f[2]
                +coeff_weno5_alpha[2][1]*f[3]
                +coeff_weno5_alpha[2][2]*f[4];
                
                double tau=std::fabs(is2-is0);
                
                alpha0=coeff_weno5_c[0]*(1.0+std::pow(tau/(is0+ss),p));
                alpha1=coeff_weno5_c[1]*(1.0+std::pow(tau/(is1+ss),p));
                alpha2=coeff_weno5_c[2]*(1.0+std::pow(tau/(is2+ss),p));
                
                
                alphaSum=alpha0+alpha1+alpha2;
                
                omega0=alpha0/alphaSum;
                omega1=alpha1/alphaSum;
                omega2=alpha2/alphaSum;
                
                pOut[idx_rf]=omega0*q30+omega1*q31+omega2*q32;
            }
        }
    }
}

void nuc3d::weno5z::weno5znBR(const Field & fieldIN,
                                const Field & boundaryR,
                                Field & fieldOUT,
                                const int tilesize)
{
    double *pIn=fieldIN.getDataPtr();
    double *pOut=fieldOUT.getDataPtr();
    double *pBND=boundaryR.getDataPtr();
    
    double omega0,omega1,omega2;
    double alpha0,alpha1,alpha2,alphaSum;
    double is0,is1,is2;
    double q30,q31,q32;
    
    int nx=fieldIN.getSizeX();
    int ny=fieldIN.getSizeY();
    int nz=fieldIN.getSizeZ();
    
    int nx0=fieldOUT.getSizeX();
    int ny0=fieldOUT.getSizeY();
    int nz0=fieldOUT.getSizeZ();
    
    int nxBND=boundaryR.getSizeX();
    int nyBND=boundaryR.getSizeY();
    int nzBND=boundaryR.getSizeZ();
    
    int ibeg=nx-tilesize;
    int iend=nx;
    int jbeg=0;
    int jend=ny;
    int kbeg=0;
    int kend=nz;
    
    double f[5];
    
    for(int k=kbeg;k<kend;k++)
    {
        for(int j=jbeg;j<jend;j++)
        {
            for(int i=ibeg;i<iend;i++)
            {
                int idx_rf=nx0*ny0*k+nx0*j+i+1;
                
                for(int z=-2;z<=2;z++)
                {
                    if((i-z+1)>=nx)
                    {
                        int idx_BND=nxBND*nyBND*k+nxBND*j+i-z+1-nx;
                        
                        f[2+z]=pBND[idx_BND];
                    }
                    else
                    {
                        int idx_f=nx*ny*k+nx*j+i-z+1;
                        f[2+z]=pIn[idx_f];
                    }
                }
                
                
                is0= coeff_weno5_gamma0*std::pow((    f[0]-2.0*f[1]+    f[2]),p)
                +coeff_weno5_gamma1*std::pow((    f[0]-4.0*f[1]+3.0*f[2]),p);
                
                is1= coeff_weno5_gamma0*std::pow((    f[1]-2.0*f[2]+    f[3]),p)
                +coeff_weno5_gamma1*std::pow((    f[1]-             f[3]),p);
                
                is2= coeff_weno5_gamma0*std::pow((    f[2]-2.0*f[3]+    f[4]),p)
                +coeff_weno5_gamma1*std::pow((3.0*f[2]-4.0*f[3]+    f[4]),p);
                
                
                q30= coeff_weno5_alpha[0][0]*f[0]
                +coeff_weno5_alpha[0][1]*f[1]
                +coeff_weno5_alpha[0][2]*f[2];
                
                q31= coeff_weno5_alpha[1][0]*f[1]
                +coeff_weno5_alpha[1][1]*f[2]
                +coeff_weno5_alpha[1][2]*f[3];
                
                q32= coeff_weno5_alpha[2][0]*f[2]
                +coeff_weno5_alpha[2][1]*f[3]
                +coeff_weno5_alpha[2][2]*f[4];
                
                
                double tau=std::fabs(is2-is0);
                
                alpha0=coeff_weno5_c[0]*(1.0+std::pow(tau/(is0+ss),p));
                alpha1=coeff_weno5_c[1]*(1.0+std::pow(tau/(is1+ss),p));
                alpha2=coeff_weno5_c[2]*(1.0+std::pow(tau/(is2+ss),p));
                
                
                alphaSum=alpha0+alpha1+alpha2;
                
                omega0=alpha0/alphaSum;
                omega1=alpha1/alphaSum;
                omega2=alpha2/alphaSum;
                
                pOut[idx_rf]=omega0*q30+omega1*q31+omega2*q32;
            }
        }
    }
}



#endif
