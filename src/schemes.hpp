//
//  schemes.hpp
//  NUC3d
//
//  Created by Jun Peng on 15/12/14.
//  Copyright © 2015年 Jun Peng. All rights reserved.
//

#ifndef schemes_hpp
#define schemes_hpp

namespace nuc3d
{
    const double coeff_weno5_alpha[3][3] = {
        {1.0/3.0,-7.0/6.0,11.0/6.0},
        {-1.0/6.0,5.0/6.0,1.0/3.0},
        {1.0/3.0,5.0/6.0,-1.0/6.0}
    };
    
    
    const double coeff_weno7_alpha[4][4] = {
        {-3.0/12.0,13.0/12.0,-23.0/12.0,25.0/12.0},
        {1.0/12.0,-5.0/12.0,13.0/12.0,3.0/12.0},
        {-1.0/12.0,7.0/12.0,7.0/12.0,-1.0/12.0},
        {3.0/12.0,13.0/12.0,-5.0/12.0,1.0/12.0}
    };
    
    const double coeff_weno5_c[3]={0.1,0.6,0.3};
    const double coeff_crweno5_c[3]={0.2,0.5,0.3};
    const double coeff_upwindcompact_c[3]={2.0/7.0,4.0/7.0,1.0/7.0};
    const double coeff_weno7_c[4]={1.0/35.0,12.0/35.0,18.0/35.0,4.0/35.0};
    
    const double coeff_weno5_gamma0=13.0/12.0;
    const double coeff_weno5_gamma1=1.0/4.0;
    
    const double coeff_cd8_alpha[4] = { 4.0/5.0, -1.0/5.0, 4.0/105.0, -1.0/280.0};
    
    const double coeff_cd6_alpha[3] = { 3.0 / 4.0, -3.0 / 20.0,1.0 / 60.0 };
    
    const double coeff_cd4_alpha[2] = { 2.0 / 3.0, -1.0 / 12.0};
    
    const double coeff_cd2_alpha=0.5;

}

#endif /* schemes_hpp */
