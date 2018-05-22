module ExpenseTypeFormParams = {
  type state = {name: string};
  type fields = [ | `name];
  let lens = [(`name, s => s.name, (s, name) => {...s, name})];
};

let validationMessage = message =>
  switch (message) {
  | None => ReasonReact.nullElement
  | Some(msg) => <span className="invalid"> (ReactUtils.sloc(msg)) </span>
  };

module EditExpenseTypeForm = ReForm.Create(ExpenseTypeFormParams);

let component = ReasonReact.statelessComponent("ExpenseTypeEdit");

let make =
    (
      ~expenseType: option(ExpenseType.t)=None,
      ~onSubmit,
      ~expenseTypes,
      _children,
    ) => {
  ...component,
  render: _self => {
    let hasDuplicateName = name => {
      let duplicates =
        expenseTypes |> List.filter((c: ExpenseType.t) => c.name === name);
      let isDuplicate = duplicates |> List.length > 0;
      isDuplicate ? Some("validation.duplicate") : None;
    };
    let isUnique = (original: option(ExpenseType.t), new_) =>
      switch (original) {
      | None => hasDuplicateName(new_)
      | Some(disc) =>
        if (disc.name === new_) {
          None;
        } else {
          hasDuplicateName(new_);
        }
      };
    <EditExpenseTypeForm
      onSubmit
      initialState=(
        switch (expenseType) {
        | None => {name: ""}
        | Some(disc) => {name: disc.name}
        }
      )
      schema=[(`name, Custom(v => v.name |> isUnique(expenseType)))]>
      ...(
           ({handleSubmit, handleChange, form, getErrorForField}) => {
             let field =
                 (label, value, fieldType: ExpenseTypeFormParams.fields) =>
               <div className="field-input">
                 <label>
                   (ReactUtils.sloc(label))
                   <input
                     value
                     onChange=(
                       ReForm.Helpers.handleDomFormChange(
                         handleChange(fieldType),
                       )
                     )
                   />
                 </label>
                 (validationMessage(getErrorForField(fieldType)))
               </div>;
             <form
               onSubmit=(ReForm.Helpers.handleDomFormSubmit(handleSubmit))>
               (field("expenseType.name", form.values.name, `name))
               <Button _type="submit" label="action.done" local=true />
             </form>;
           }
         )
    </EditExpenseTypeForm>;
  },
};