import { createStructuredSelector } from 'reselect'

export const editGridOverlaySelector = createStructuredSelector({
    grid: state => state.canvas.map.mapData.grid
});