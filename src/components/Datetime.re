[%bs.raw {|require('react-datetime/css/react-datetime.css')|}];
[@bs.module] external reactClass : ReasonReact.reactClass = "react-datetime";
[@bs.obj]
external makeProps :
  (
    ~className: string=?,
    ~locale: string=?,
    ~value: Js.Date.t=?,
    ~onBlur: ReasonReact.Callback.t(MomentRe.Moment.t)=?,
    ~onFocus: ReasonReact.Callback.t(ReactEventRe.Focus.t)=?,
    ~onChange: ReasonReact.Callback.t(MomentRe.Moment.t)=?,
    unit
  ) =>
  _ =
  "";

let make =
    (
      ~className: option(string)=?,
      ~locale: option(string)=?,
      ~value: option(Js.Date.t)=?,
      ~onBlur: option(ReasonReact.Callback.t(MomentRe.Moment.t))=?,
      ~onFocus: option(ReasonReact.Callback.t(ReactEventRe.Focus.t))=?,
      ~onChange: option(ReasonReact.Callback.t(MomentRe.Moment.t))=?,
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props=
      makeProps(
        ~className?,
        ~locale?,
        ~value?,
        ~onBlur?,
        ~onFocus?,
        ~onChange?,
        (),
      ),
    children,
  );