function r = getRms(input, W)
    r =sqrt(movmean(input.^2, W));
end
