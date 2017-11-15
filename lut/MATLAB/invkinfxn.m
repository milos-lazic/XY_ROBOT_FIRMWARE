function [th1, th3, th2_, th4] = invkinfxn(P)

    px = P(1);
    py = P(2);
    
    % define constants (device specific)
    L1 = 220; % mm
    L2 = 220; % mm
    L3 = 220; % mm
    L4 = 220; % mm
    L5 = 54;  % mm
    L6 = 100; % mm
    L7 = 249.4338; % mm
    alpha = (180 - 117.31) * pi / 180; % degrees to radians
    beta = tan( (L5*sin(alpha)) / (220 + L5*cos(alpha))); % radians
    
    % determine theta 1 and use first solution
    c = (L7^2 - px^2 - py^2 - L1^2) / (-2*L1);
    a = px;
    b = py;
    th1 = atan2(b,a) + atan2( sqrt(a^2 + b^2 - c^2), c);
    
    % intermediate step - determine theta 2
    th2 = acos( (px - L1*cos(th1)) / L7);
    
    % intermediate step - determine theta 2 prime
    th2_ = th2 - beta;
    
    % intermediate step - determine coordinates of intermediate point Q;
    % from origin of axis A with respect to axis A
    qx = L1*cos(th1) + L2*cos(th2_);
    qy = L1*sin(th1) + L2*sin(th2_);
    
    % determine theta 3 and use second solution
    a = qx - L6;
    b = qy;
    c = ( L4^2 - (qx - L6)^2 - qy^2 - L3^2) / (-2*L3);
    th3 = atan2(b,a) - atan2( sqrt(a^2 + b^2 - c^2), c);
    
    
    th1 = th1 * 180 / pi;
    th3 = th3 * 180 / pi;
    th2_ = th2_ * 180 / pi;
    
    th4 = acosd( (qx - L6 - L3*cosd(th3)) / L4 );
    
end
