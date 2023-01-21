clear;
clc;

wij = [];


gammas = [0.44, 0.2, 0.24, 0.12];

d = [];

N = [{1,2,3}, {1,2,3}, {2,3,4}, {4,3}];

for i=1:4
    for j = 1:4
        if (i ~= j)
           wij = [wij 1/gammas(i)*min(gammas(i)/d(i),gammas(j)/d(j))]
        end
    end
end