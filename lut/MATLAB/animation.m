close all; clear; clc;

[th1, th3, th2_, th4] = invkinfxn([-89.7;427.90]);

alpha = 180 - 117.31;
R_AW = transpose([1 0; 0 -1]);
P_AW = transpose([-89.7 427.90]);

for y=0:1:160

    cla;
    
    P_AP = P_AW + R_AW*[0;y];

    [th1, th3, th2_, th4] = invkinfxn(P_AP);
    
    plot([0 220*cosd(th1)], [0 220*sind(th1)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([220*cosd(th1) 220*cosd(th1)+220*cosd(th2_)], [220*sind(th1) 220*sind(th1)+220*sind(th2_)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([100 220*cosd(th3)+100], [0 220*sind(th3)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([220*cosd(th3)+100 220*cosd(th3)+100+220*cosd(th4)], [220*sind(th3) 220*sind(th3)+220*sind(th4)], 'LineWidth', 2, 'Color', 'blue'); hold on;
    plot([220*cosd(th1)+220*cosd(th2_) 220*cosd(th1)+220*cosd(th2_)+54*cosd(th2_+alpha)], [220*sind(th1)+220*sind(th2_) 220*sind(th1)+220*sind(th2_)+54*sind(th2_+alpha)], 'LineWidth', 2, 'Color', 'blue');
    xlim([-250 350])
    ylim([   0 500])
    grid on;
    
    pause(0.01);
    
end

% for y=0:1:150
% 
%     cla;
%     
%     P_AP = P_AW + R_AW*[250;y];
% 
%     [th1, th3, th2_, th4] = invkinfxn(P_AP);
%     
%     plot([0 220*cosd(th1)], [0 220*sind(th1)], 'LineWidth', 2, 'Color', 'blue'); hold on;
%     plot([220*cosd(th1) 220*cosd(th1)+220*cosd(th2_)], [220*sind(th1) 220*sind(th1)+220*sind(th2_)], 'LineWidth', 2, 'Color', 'blue'); hold on;
%     plot([100 220*cosd(th3)+100], [0 220*sind(th3)], 'LineWidth', 2, 'Color', 'blue'); hold on;
%     plot([220*cosd(th3)+100 220*cosd(th3)+100+220*cosd(th4)], [220*sind(th3) 220*sind(th3)+220*sind(th4)], 'LineWidth', 2, 'Color', 'blue'); hold on;
%     plot([220*cosd(th1)+220*cosd(th2_) 220*cosd(th1)+220*cosd(th2_)+54*cosd(th2_+alpha)], [220*sind(th1)+220*sind(th2_) 220*sind(th1)+220*sind(th2_)+54*sind(th2_+alpha)], 'LineWidth', 2, 'Color', 'blue');
%     xlim([-250 350])
%     ylim([   0 500])
%     grid on;
%     
%     pause(0.01);
%     
% end
% 
% for x=250:-1:0
% 
%     cla;
%     
%     P_AP = P_AW + R_AW*[x;150];
% 
%     [th1, th3, th2_, th4] = invkinfxn(P_AP);
%     
%     plot([0 220*cosd(th1)], [0 220*sind(th1)], 'LineWidth', 2, 'Color', 'blue'); hold on;
%     plot([220*cosd(th1) 220*cosd(th1)+220*cosd(th2_)], [220*sind(th1) 220*sind(th1)+220*sind(th2_)], 'LineWidth', 2, 'Color', 'blue'); hold on;
%     plot([100 220*cosd(th3)+100], [0 220*sind(th3)], 'LineWidth', 2, 'Color', 'blue'); hold on;
%     plot([220*cosd(th3)+100 220*cosd(th3)+100+220*cosd(th4)], [220*sind(th3) 220*sind(th3)+220*sind(th4)], 'LineWidth', 2, 'Color', 'blue'); hold on;
%     plot([220*cosd(th1)+220*cosd(th2_) 220*cosd(th1)+220*cosd(th2_)+54*cosd(th2_+alpha)], [220*sind(th1)+220*sind(th2_) 220*sind(th1)+220*sind(th2_)+54*sind(th2_+alpha)], 'LineWidth', 2, 'Color', 'blue');
%     xlim([-250 350])
%     ylim([   0 500])
%     grid on;
%     
%     pause(0.01);
%     
% end
% 
% 
% for y=150:-1:0
% 
%     cla;
%     
%     P_AP = P_AW + R_AW*[0;y];
% 
%     [th1, th3, th2_, th4] = invkinfxn(P_AP);
%     
%     plot([0 220*cosd(th1)], [0 220*sind(th1)], 'LineWidth', 2, 'Color', 'blue'); hold on;
%     plot([220*cosd(th1) 220*cosd(th1)+220*cosd(th2_)], [220*sind(th1) 220*sind(th1)+220*sind(th2_)], 'LineWidth', 2, 'Color', 'blue'); hold on;
%     plot([100 220*cosd(th3)+100], [0 220*sind(th3)], 'LineWidth', 2, 'Color', 'blue'); hold on;
%     plot([220*cosd(th3)+100 220*cosd(th3)+100+220*cosd(th4)], [220*sind(th3) 220*sind(th3)+220*sind(th4)], 'LineWidth', 2, 'Color', 'blue'); hold on;
%     plot([220*cosd(th1)+220*cosd(th2_) 220*cosd(th1)+220*cosd(th2_)+54*cosd(th2_+alpha)], [220*sind(th1)+220*sind(th2_) 220*sind(th1)+220*sind(th2_)+54*sind(th2_+alpha)], 'LineWidth', 2, 'Color', 'blue');
%     xlim([-250 350])
%     ylim([   0 500])
%     grid on;
%     
%     pause(0.01);
%     
% end