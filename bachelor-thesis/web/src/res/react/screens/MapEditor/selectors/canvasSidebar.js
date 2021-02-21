import { createStructuredSelector } from 'reselect'

export const canvasSidebarSelector = createStructuredSelector({
    tool: state => state.canvasTool.tool,
    styles: state => state.canvas.map.mapData.styles
});