
%%

time = time-time(1);
%%

%p = polyfit(time(1:1000), distance(1:1000), 1 );
%y2 = p(1) * x + p(2);
k = time(1:1000)\distance(1:1000) % just for compare
distance = distance - k*time;

distance = distance(1:8030);
height = height(1:8030);
time = time(1:8030);
uavangle = uavangle(1:8030);


%%
plot(time,distance,time,height,time,uavangle);
legend('User distance','Quadcopter height','User centerline offset');
ylabel('[cm]');
xlabel('running time [s]');



%%
figure
subplot(3,1,1)       % add first plot in 2 x 2 grid
plot(time,height)           % line plot
title('Quadcopter height')
ylim([40 70])
ylabel('[cm]')
grid on

subplot(3,1,2)       % add second plot in 2 x 2 grid
plot(time,distance)
title('User distance')
ylim([-70 70])
ylabel('[cm]')
grid on

subplot(3,1,3)       % add third plot in 2 x 2 grid
plot(time,uavangle)           % stem plot
title('User centerline offset')
ylim([-6 6])
ylabel('[cm]')
xlabel('seconds')
grid on
