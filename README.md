# HMSpice

HMSpice is an electronic circuit simulator for circuits containing voltage, current sources (independent/dependent) and resistors, i.e., for **purely resistive circuits**. This simulator is built by taking inspiration from the "[Circuit Simulation](https://onlinelibrary.wiley.com/doi/book/10.1002/9780470561218)" book by Farid N. Najm. This work was part of my senior design project at [Shiv Nadar University](https://snu.edu.in/home) in the Monsoon 2022 semester.

## Working
HMSpice uses Modified Nodal Analysis (MNA) to find all the nodal voltages and required currents across the branches. It builds the MNA and RHS matrices in O(n) time complexity. It goes through the netlist, checks for any error in the netlist, and makes the matrices, on the fly, in linear time, using the element stamp of each component.

The contribution of every element to the matrix equation is described by employing a template called an element stamp. Every element has different stamps based on their contribution to the matrices and on which group they belong to.

The simulator uses the [Eigen](https://eigen.tuxfamily.org/) library to implement the solver. We have used LU factorization to solve the equation.

## Installation and Running HMSpice
- Requirement: GCC Compiler (or any other C++ compiler)

### Installation
- Download the project from GitHub
- Compile all the files

### Running
- Type the netlist in *circuit.hms* file
- Run the executable file from terminal to get all nodal voltages and required currents

## Accepted Syntax of Circuit Elements
- Independent Current Source: `I<string> <node.+> <node.-> <value> [G2] `
- Dependent Current Source: `Ic<string> <node.+> <node.-> <factor> <variable> <circuitElement>`
- Independent Voltage Source: `V<string> <node.+> <node.-> <value>`
- Dependent Voltage Source: `Vc<string> <node.+> <node.-> <factor> <variable> <circuitElement>`
- Resistor: `R<string> <node.+> <node.-> <value> [G2]`

`G2`, for group 2, is an optional field that tells the simulator that the user is interested in knowing the current across the circuit element. By default, all voltage sources are group 2.

Accepted values for `variable` are either `V` or `I`, which tell the simulator that the source is dependent on that particular variable of the `circuitElement`.

`<node.+>`, `<node.->` takes string and `value`, `factor` takes value in normal integer, decimal or exponential form but doesn't allow multiplier.

## UML Diagrams
This project contains one structure: *CircuitElement*, and three classes: *Parser*, *Edge* and *Node*. Following is the UML diagram of each.

![CircuitElement Structure](/Class%20Diagram/CircuitElement.png)

![Parser Class](/Class%20Diagram/Parser.png)

![Node Class](/Class%20Diagram/Node.png)

![Edge Class](/Class%20Diagram/Edge.png)

## Constraints
- One of the nodes in the netlist must be 0 (string)
- Current controlling variable's element must be in group 2
- Controlling variable cannot be dependent on another dependent variable
- Values in the netlist cannot accept multipliers (give actual values or in exponential)
- Inline comments are best avoided (if required, explicitly state the group before commenting)

## List of Errors and Warnings
### Errors:
- Unknown element
- Circuit must contain ground (0)
- Checks whether the controlling element is present in the netlist
- Controlled source cannot be cascaded
- Reviews for legal input value or factor
- Illegal controlling variable argument
- Netlist not available

### Warnings
- Checks and makes sure that the controlled element is in group 2 if the controlling variable is current
- Two nodes of a component cannot be the same (Omits the component)
- Mention the correct group (by default, assigns group 1)

## Credits
We are thankful to the **Eigen** library team for developing and maintaining the library, because of which the equation could be solved so efficiently.
