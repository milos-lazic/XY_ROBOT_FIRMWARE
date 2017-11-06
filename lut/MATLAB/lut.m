clear; clc;


% center of workspace
px = 50; % dummy tooltip coordinate x (for testing)
py = 319.95;  % dummy tooltip coordinate y (for testing)


% Rotation matrix; axes B (workspace axes) with respect to axes A
% (manipulator base axes)
R_AB = transpose([1 0; 0 -1]);

% Position vector; origin axis A to origin axis B; with respect to axis A
P_AB = transpose([-229.40 427.90]);


[th1, th3] = invkinfxn([-89.7, 427.90])


