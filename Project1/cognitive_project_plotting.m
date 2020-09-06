clear
clc

%read data
a1=csvread('test_results PU 0.000000 coop 0.csv');%to read PU 0.00 no coop
a2=csvread('test_results PU 0.150000 coop 0.csv');%to read PU 0.15 no coop
a3=csvread('test_results PU 0.250000 coop 0.csv');%to read PU 0.25 no coop
a4=csvread('test_results PU 0.000000 coop 1.csv');%to read PU 0.00 coop
a5=csvread('test_results PU 0.150000 coop 1.csv');%to read PU 0.15 coop
a6=csvread('test_results PU 0.250000 coop 1.csv');%to read PU 0.25 coop
%read parameters
M=a1(1,1);
N=a1(1,2);
T=a1(1,3);
LTDDF=a1(1,4);
Sn_range=a1(1,5);

graph=zeros(1, 9);
%plot user vs time
graph(1)=figure('name', 'Figure 1: SU 4 vs. time','numbertitle','off');
plot(1:T, a1(2,1:T))
title('SU 4 vs. time')
grid on
xlabel('Time (timeslot) ');
ylabel('S_4');
xlim([1 T]);
ylim([-0.1 LTDDF]);
set(findall(gca, 'Type', 'Line'),'LineWidth',2);

%plot parameters vs M
graph(2)=figure('name', 'Figure 2: Throughput Average','numbertitle','off');
graph(3)=figure('name', 'Figure 3: Utilization Average','numbertitle','off');
graph(4)=figure('name', 'Figure 4: PU Interference Ratio','numbertitle','off');

for i=2:4
    figure(graph(i));
    plot(5:5:M, a1(i+1,5:5:M),'rx', 5:5:M, a2(i+1,5:5:M),'kx', 5:5:M, a3(i+1,5:5:M), 'bx', 5:5:M, a4(i+1,5:5:M),'go', 5:5:M, a5(i+1,5:5:M),'co', 5:5:M, a6(i+1,5:5:M), 'mo', 'MarkerSize', 12);
    hold on;
    plot(1:M, a1(i+1,1:M),'-r', 1:M, a2(i+1,1:M),'-k', 1:M, a3(i+1,1:M), '-b', 1:M, a4(i+1,1:M),'--g', 1:M, a5(i+1,1:M),'--c', 1:M, a6(i+1,1:M), '--m');
    grid on
    xlabel('Bands M');
    xlim([1 M]);
    ylim([0 1]);
    switch i
        case 2
            title('Throughput Average')
            ylabel('Y_m')
        case 3
            title('Utilization Average')
            ylabel('U_m');
        case 4
            title('PU Interference Ratio')
            ylabel('\phi_m')            
    end
    legend('PU\_active=0.0, without coop', 'PU\_active=0.15,  without coop', 'PU\_active=0.25,  without coop','PU\_active=0.0,  with coop', 'PU\_active=0.15,  with coop', 'PU\_active=0.25,  with coop')
    set(findall(gca, 'Type', 'Line'),'LineWidth',2);
end

%plot vs SU
graph(5)=figure('name', 'Figure 5: Successful Transission Average','numbertitle','off');
graph(6)=figure('name', 'Figure 6: Failure Transmission Average','numbertitle','off');
for i=5:6
    figure(graph(i));
    plot(1:N, a1(i+1,1:N),'-rx', 1:N, a2(i+1,1:N),'-kx', 1:N, a3(i+1,1:N), '-bx', 1:N, a4(i+1,1:N),'--go', 1:N, a5(i+1,1:N),'--co', 1:N, a6(i+1,1:N), '--mo', 'MarkerSize', 12)
    grid on
    xlabel('SU N');
    xlim([1 N]);
    ylim([0 LTDDF]);
    switch i
        case 5
            title('Successful Transission Average')
            ylabel('S_n')
        case 6
            title('Failure Transmission Average')
            ylabel('C_n')
    end
    legend('PU\_active=0.0, without coop', 'PU\_active=0.15,  without coop', 'PU\_active=0.25,  without coop','PU\_active=0.0,  with coop', 'PU\_active=0.15,  with coop', 'PU\_active=0.25,  with coop')
    set(findall(gca, 'Type', 'Line'),'LineWidth',2);
end

graph(7)=figure('name', 'Figure 7: False Alarm Average','numbertitle','off');
graph(8)=figure('name', 'Figure 8: Misdetection Average','numbertitle','off');
for i=7:8
    figure(graph(i));
    plot(1:N, a1(i+1,1:N),'-rx', 1:N, a2(i+1,1:N),'-ko', 1:N, a3(i+1,1:N), '--bx', 1:N, a4(i+1,1:N),'--go', 1:N, a5(i+1,1:N),'-co', 1:N, a6(i+1,1:N), '--mx', 'MarkerSize', 12)
    grid on
    xlabel('SU N');
    xlim([1 N]);
    ylim([0 max([max(a1(i+1, :)) max(a2(i+1, :)) max(a3(i+1, :)) max(a4(i+1, :)) max(a5(i+1, :)) max(a6(i+1, :))]) *2]);
    switch i
        case 7
            title('False Alarm Average')
            ylabel('F_n')
        case 8
            title('Misdetection Average')
            ylabel('D_n')
    end
    legend('PU\_active=0.0, without coop', 'PU\_active=0.15,  without coop', 'PU\_active=0.25,  without coop','PU\_active=0.0,  with coop', 'PU\_active=0.15,  with coop', 'PU\_active=0.25,  with coop')
    set(findall(gca, 'Type', 'Line'),'LineWidth',2);
end

graph(9)=figure('name', 'Figure 9: Successful Transmission vs. Load Changing','numbertitle','off');
plot(a1(10,1:Sn_range), a1(11,1:Sn_range),'-rx', a2(10,1:Sn_range), a2(11,1:Sn_range),'-kx', a3(10,1:Sn_range), a3(11,1:Sn_range), '-bx', a4(10,1:Sn_range), a4(11,1:Sn_range),'--go', a5(10,1:Sn_range), a5(11,1:Sn_range),'--co', a6(10,1:Sn_range), a6(11,1:Sn_range), '--mo', 'MarkerSize', 12)
title('Successfully Acquired Bands')
grid on
xlabel('load');
ylabel('S')
xlim([0 max(a1(10,:))+1]);
ylim([0 LTDDF]);
legend('PU\_active=0.0, without coop', 'PU\_active=0.15,  without coop', 'PU\_active=0.25,  without coop','PU\_active=0.0,  with coop', 'PU\_active=0.15,  with coop', 'PU\_active=0.25,  with coop')
set(findall(gca, 'Type', 'Line'),'LineWidth',2);