close all; clear; clc;

[th1, th3, th2_, th4] = invkinfxn([-89.7;427.90]);

alpha = 180 - 117.31;
R_AW = transpose([1 0; 0 -1]);
P_AW = transpose([-89.7 427.90]);

ll = 220; % link length
ee = 54;  % end effector length


for x=0:1:250

    cla;
    
    P_AP = P_AW + R_AW*[x;0];

    [th1, th3, th2_, th4] = invkinfxn(P_AP);
    
    plot([0 ll*cosd(th1)], [0 ll*sind(th1)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([ll*cosd(th1) ll*cosd(th1)+ll*cosd(th2_)], [ll*sind(th1) ll*sind(th1)+ll*sind(th2_)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([100 ll*cosd(th3)+100], [0 ll*sind(th3)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([ll*cosd(th3)+100 ll*cosd(th3)+100+ll*cosd(th4)], [ll*sind(th3) ll*sind(th3)+ll*sind(th4)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([ll*cosd(th1)+ll*cosd(th2_) ll*cosd(th1)+ll*cosd(th2_)+ee*cosd(th2_+alpha)], [ll*sind(th1)+ll*sind(th2_) ll*sind(th1)+ll*sind(th2_)+ee*sind(th2_+alpha)], 'LineWidth', 2, 'Color', 'blue');
    xlim([-250 350])
    ylim([   0 500])
    grid on;
    
    pause(0.01);
    
end

for y=0:1:150

    cla;
    
    P_AP = P_AW + R_AW*[250;y];

    [th1, th3, th2_, th4] = invkinfxn(P_AP);
    
    plot([0 ll*cosd(th1)], [0 ll*sind(th1)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([ll*cosd(th1) ll*cosd(th1)+ll*cosd(th2_)], [ll*sind(th1) ll*sind(th1)+ll*sind(th2_)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([100 ll*cosd(th3)+100], [0 ll*sind(th3)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([ll*cosd(th3)+100 ll*cosd(th3)+100+ll*cosd(th4)], [ll*sind(th3) ll*sind(th3)+ll*sind(th4)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([ll*cosd(th1)+ll*cosd(th2_) ll*cosd(th1)+ll*cosd(th2_)+ee*cosd(th2_+alpha)], [ll*sind(th1)+ll*sind(th2_) ll*sind(th1)+ll*sind(th2_)+ee*sind(th2_+alpha)], 'LineWidth', 2, 'Color', 'blue');
    xlim([-250 350])
    ylim([   0 500])
    grid on;
    
    pause(0.01);
    
end

for x=250:-1:0

    cla;
    
    P_AP = P_AW + R_AW*[x;150];

    [th1, th3, th2_, th4] = invkinfxn(P_AP);
    
    plot([0 ll*cosd(th1)], [0 ll*sind(th1)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([ll*cosd(th1) ll*cosd(th1)+ll*cosd(th2_)], [ll*sind(th1) ll*sind(th1)+ll*sind(th2_)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([100 ll*cosd(th3)+100], [0 ll*sind(th3)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([ll*cosd(th3)+100 ll*cosd(th3)+100+ll*cosd(th4)], [ll*sind(th3) ll*sind(th3)+ll*sind(th4)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([ll*cosd(th1)+ll*cosd(th2_) ll*cosd(th1)+ll*cosd(th2_)+ee*cosd(th2_+alpha)], [ll*sind(th1)+ll*sind(th2_) ll*sind(th1)+ll*sind(th2_)+ee*sind(th2_+alpha)], 'LineWidth', 2, 'Color', 'blue');
    xlim([-250 350])
    ylim([   0 500])
    grid on;
    
    pause(0.01);
    
end


for y=150:-1:0

    cla;
    
    P_AP = P_AW + R_AW*[0;y];

    [th1, th3, th2_, th4] = invkinfxn(P_AP);
    
    plot([0 ll*cosd(th1)], [0 ll*sind(th1)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([ll*cosd(th1) ll*cosd(th1)+ll*cosd(th2_)], [ll*sind(th1) ll*sind(th1)+ll*sind(th2_)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([100 ll*cosd(th3)+100], [0 ll*sind(th3)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([ll*cosd(th3)+100 ll*cosd(th3)+100+ll*cosd(th4)], [ll*sind(th3) ll*sind(th3)+ll*sind(th4)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([ll*cosd(th1)+ll*cosd(th2_) ll*cosd(th1)+ll*cosd(th2_)+ee*cosd(th2_+alpha)], [ll*sind(th1)+ll*sind(th2_) ll*sind(th1)+ll*sind(th2_)+ee*sind(th2_+alpha)], 'LineWidth', 2, 'Color', 'blue');
    xlim([-250 350])
    ylim([   0 500])
    grid on;
    
    pause(0.01);
    
end