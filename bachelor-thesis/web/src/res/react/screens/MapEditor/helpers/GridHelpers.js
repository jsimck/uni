const GridHelpers = {
    computeGrid: (point, gridSize) => {
        return Math.round(point / gridSize) * gridSize;
    }
};

export default GridHelpers