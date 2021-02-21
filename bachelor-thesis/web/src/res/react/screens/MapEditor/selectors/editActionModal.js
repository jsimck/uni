import { createStructuredSelector } from 'reselect'

export const editActionModalSelector = createStructuredSelector({
    visible: state => state.editActionModal.visible,
    svg: state => state.editActionModal.svg,
    index: state => state.editActionModal.index,
    map: state => state.canvas.map
});