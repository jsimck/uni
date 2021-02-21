import React, { Component } from 'react'

class OptionsSidebarBlock extends Component {
    render() {
        var block = null;

        // Show only if show prop is true
        if (this.props.show) {
            block = (
                <div>
                    {this.props.children}
                </div>
            );
        }

        // Render block
        return block;
    }
}

OptionsSidebarBlock.defaultProps = {
    show: true
};

OptionsSidebarBlock.propTypes = {
    children: React.PropTypes.any,
    show: React.PropTypes.bool
};

export default OptionsSidebarBlock