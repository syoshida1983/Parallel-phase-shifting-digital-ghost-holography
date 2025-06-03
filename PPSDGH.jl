using CSV
using DataFrames
using Statistics
using Images
using Hadamard

"""
    LUT(n::Int)

return look-up tables containing the row and column order
sorted by frequency for the Walsh matrix.
"""
function LUT(n::Int)
    rows = Array{Int}(undef, n*n, 1)
    cols = Array{Int}(undef, n*n, 1)
    k = 1

    for i ∈ 1:n, j ∈ 1:i
        cols[k] = j
        rows[k] = i - j + 1
        k += 1
    end

    for i ∈ reverse(1:n-1)
        m = 1
        for j ∈ n-i:n-1
            cols[k] = j + 1
            rows[k] = n - m + 1
            k += 1
            m += 1
        end
    end
    
    return rows, cols
end

"""
    PPSDGH(y, δ, rows, cols)

return wavefront x for the measurements y.

# Arguments
- `y`: measurements.
- `δ`: phase difference.
- `n`: image size.
- `rows`: row order for the Walsh matrix
- `cols`: column order for the Walsh matrix
"""
function PPSDGH(y, δ, n, rows, cols)
    W = walsh(n)
    x = zeros(ComplexF64, n, n)

    for i ∈ 1:length(y)
        φ = (W[:,cols[i]]*W[rows[i],:]' .+ 1)δ/2
        x = x + y[i]*exp.(-im*φ)
    end

    return x
end

function main()
    csvfile = "measurements.csv"    # input measurements
    imgfile = "phase.bmp"   # output phase image
    n = 128 # image size
    δ = π   # phase difference

    df = CSV.read(csvfile, DataFrame; header=false)
    y = df[:,1] + df[:,2]*im
    y = y .- mean(y)
    rows, cols = LUT(n)
    x = PPSDGH(y, δ, n, rows, cols)
    ϕ = angle.(x)
    save(imgfile, (ϕ .+ π)/(2π))
end

main()
