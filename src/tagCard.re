open Util;

let component = ReasonReact.statelessComponent("TagCard");

/* Here, the button should push to /NewOrder?customerName=john and the view changes */
let make = (~tag: string, ~onSelect, _children) => {
  ...component,
  render: _self =>
    <div onClick=((_) => onSelect(tag)) id="tag-card"> (s(tag)) </div>,
};