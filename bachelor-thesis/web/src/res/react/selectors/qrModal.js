import { createStructuredSelector } from 'reselect'

export const qrModalSelector = createStructuredSelector({
    visible: state => state.qrModal.visible,
    data: state => state.qrModal.data
});