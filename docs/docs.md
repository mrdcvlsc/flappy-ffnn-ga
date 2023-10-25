# Documentation

When choosing inputs for neural networks like this, it is better to select inputs that are independent of each other.

# Genetic Algorithm

## Requirements

1. Fitness function - calculates the measure of how well the bird is performing in the environment.
    - How long the bird survived.
    - How close the bird was to the gap when it died.

2. `MUTATION_CUT_OFF` - A percentage that indicates how many of the highest fitness birds you want to keep when all the birds have died.

3. `MUTATION_BAD_TO_KEEP` - A percentage that indicates how many of the **MUTATED BAD BIRDS** you want to keep.

## Steps

1. Generate a population of birds with random weights initialized in their neural network.

2. Let the birds run in the environment.

3. When all the birds die, sort them from the highest fitness to the lowest fitness.

4. Isolate the birds with the highest fitness (**GOOD BIRDS**) from the low fitness birds (**BAD BIRDS**).

5. Mutate some of the weights of the **BAD BIRDS** slightly, we will call then now **MUTATED BAD BIRDS**.

6. Choose a `MUTATION_BAD_TO_KEEP` percentage of random birds to keep from the **MUTATED BAD BIRDS**.

7. Splice/join the **GOOD BIRDS** and the **MUTATED BAD BIRDS** back into the bird population collection.

8. Breed from the **GOOD BIRDS** to fill up the missing number of birds in the resulting population. The way breeding works in this context is by mixing up random weights of two different **GOOD BIRDS**.
