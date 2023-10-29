# Documentation

- When selecting inputs for neural networks like this, it is better to choose inputs that are independent of each other.
- Normalized inputs should have a one-to-one mapping (a function). If this is not followed, the network will not perform well.
- Inputs are crucial; they should be precise, accurate, and have a clear intent. For example, in this project, when the target pipe gap point changes immediately, even if the bird is not fully outside the current pipe gap, the birds cannot perform well. However, this issue was immediately resolved by moving the target pipe gap point slightly to the left so that it changes only after the bird has safely passed over the current pipe gap.
- The network will also struggle if the problem is too difficult. Ensure that the environment is playable before implementing and integrating the network into it.
- Genetic algorithms perform well with larger populations.
- For genetic algorithms, the larger the neural network is, the longer it takes to optimize it.

# Genetic Algorithm

## Requirements

1. Fitness function - calculates how well the bird is performing in the environment.
    - How long the bird survives.
    - How close the bird is to the gap when it dies.

2. `MUTATION_CUT_OFF` - A percentage that indicates how many of the highest fitness birds to keep when all the birds have died.

3. `MUTATION_BAD_TO_KEEP` - A percentage that indicates how many of the **MUTATED BAD BIRDS** you want to keep.

## Steps

1. Generate a population of birds with randomly initialized weights in their neural network.

2. Let the birds run in the environment.

3. When all the birds have died, sort them from the highest fitness to the lowest fitness.

4. Isolate the birds with the highest fitness (**GOOD BIRDS**) from the birds with low fitness (**BAD BIRDS**).

5. Slightly mutate some of the weights of the **BAD BIRDS**; we will now call them **MUTATED BAD BIRDS**.

6. Choose a `MUTATION_BAD_TO_KEEP` percentage of random birds to keep from the **MUTATED BAD BIRDS**.

7. Combine the **GOOD BIRDS** and the **MUTATED BAD BIRDS** back into the bird population.

8. Breed from the **GOOD BIRDS** to fill up the remaining slots in the population. Breeding, in this context, involves mixing the weights of two different **GOOD BIRDS**.
