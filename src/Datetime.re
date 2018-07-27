[%bs.raw {|require('react-datetime/css/react-datetime.css')|}];
[@bs.module] external reactClass : ReasonReact.reactClass = "react-datetime";
[@bs.obj]
external makeProps :
  (
    ~className: string=?,
    ~value: string=?,
    ~onBlur: ReasonReact.Callback.t(ReactEventRe.Focus.t)=?,
    ~onFocus: ReasonReact.Callback.t(ReactEventRe.Focus.t)=?,
    ~onChange: ReasonReact.Callback.t(ReactEventRe.Focus.t)=?,
    unit
  ) =>
  _ =
  "";

let make =
    (
      ~className: option(string)=?,
      ~value: option(string)=?,
      ~onBlur: option(ReasonReact.Callback.t(ReactEventRe.Focus.t))=?,
      ~onFocus: option(ReasonReact.Callback.t(ReactEventRe.Focus.t))=?,
      ~onChange: option(ReasonReact.Callback.t(ReactEventRe.Focus.t))=?,
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=
      makeProps(~className?, ~value?, ~onBlur?, ~onFocus?, ~onChange?, ()),
    children,
  );