// Exponential Moving Average

// EMA(t) = α·X(t) + (1-α)·EMA(t-1)

#define alpha 0.5

float lastEMA = 0;
float currentEMA = 0;
float EMA(int sample)
{
    currentEMA = alpha*sample + (1-alpha)*lastEMA;
    lastEMA = currentEMA;
    return currentEMA;
}