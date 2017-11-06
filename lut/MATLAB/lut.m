clear; clc;


% % center of workspace
% px = 50; % dummy tooltip coordinate x (for testing)
% py = 319.95;  % dummy tooltip coordinate y (for testing)


% Rotation matrix; axes W (workspace axes) with respect to axes A
% (manipulator base axes)
R_AW = transpose([1 0; 0 -1]);

% Position vector; origin axis A to origin axis W; with respect to axis A
P_AW = transpose([-89.7 427.90]);

P_AP = P_AW + R_AW*[139;107];

[th1 th2] = invkinfxn( P_AP);

fileID = fopen('table.txt', 'w');

fprintf(fileID, 'struct pos table[][] =\r\n');
fprintf(fileID, '{\r\n');

X_LIMIT = 279;
Y_LIMIT = 215;

for x=0:1:10
    
    fprintf( fileID, '     { ');
    
    for y=0:1:10
        
        P_AP = P_AW + R_AW*[x;y];
        
        [th1, th3] = invkinfxn( P_AP);
        
        
        fprintf(fileID, '{%.0f, %.0f}', 1000*th1, 1000*th3);
        
        if y < 10
            fprintf(fileID, ', ');
        else
            fprintf(fileID, ' ');
        end
        
    end
    
    fprintf(fileID, '}, \r\n');
    
end

fprintf(fileID, '};\r\n');

fclose(fileID);
