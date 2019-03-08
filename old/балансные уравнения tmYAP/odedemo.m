function odedemo
x=0:0.1:20; plot(x,exp(-x).*sin(2.*x));hold on;
Y0 = [1; 0];
[T, Y] = ode113(@oscil, [0 20], Y0);
plot(T, Y(:,1), 'r.-')
hold on; plot(T, Y(:,2), 'k.:')
title('Solve equation {\ity} \prime\prime+0.5{\ity} \prime+5{\ity}= exp(-{\itt})*sin{(2*\itt})')
xlabel('\itt'); ylabel('{\ity}, {\ity} \prime ')
legend('input','coordinate', 'speed', 4);grid on; hold off
function F = oscil(t, y)
F = [y(2); -0.5*y(2)-5*y(1)+exp(-t)*sin(2*t)];