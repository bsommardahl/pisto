open ReactUtils;

let fieldsAllInvalid =
  Js.Dict.fromList([
    ("Description", false),
    ("Vendor", false),
    ("ExpenseType", false),
    ("Date", true),
    ("SubTotals", false),
    ("Total", false),
  ]);

let fieldsAllValid =
  fieldsAllInvalid
  |> Js.Dict.entries
  |> Js.Array.map(((k, _)) => (k, true))
  |> Js.Dict.fromArray;

let opposite = (b: bool) => b ? false : true;

let isExpenseValid = fields => {
  Js.log(fields);
  fields
  |> Js.Dict.entries
  |> Array.to_list
  |> List.exists(((_k, v)) => v == false)
  |> opposite;
};

type validator = {
  field: string,
  check: string => bool,
  isValid: bool,
  message: string,
};

let noEmptyString = n => n !== "";

let noNone = n => n !== "None";

let validators: list(validator) = [
  {field: "Vendor", check: noNone, message: "Requerido", isValid: true},
  {field: "ExpenseType", check: noNone, message: "Requerido", isValid: true},
  {
    field: "Description",
    check: noEmptyString,
    message: "Requerido",
    isValid: true,
  },
  {field: "Date", check: noEmptyString, message: "Requerido", isValid: true},
  {
    field: "SubTotals",
    check: n => n != "0",
    message: "Requerido",
    isValid: true,
  },
  {field: "Total", check: n => n != "0", message: "Requerido", isValid: true},
];

let isValid = (field, value) => {
  let match = validators |> List.find(v => v.field === field);
  match.check(value);
};

type state = {
  description: string,
  vendor: Vendor.t,
  expenseType: ExpenseType.t,
  date: Date.t,
  subTotals: list(Expense.SubTotal.t),
  tax: Money.t,
  total: Money.t,
  newSubtotalAmount: Money.t,
  newSubtotalTaxRate: Percent.t,
  isValid: bool,
  fields: Js.Dict.t(bool),
};

type action =
  | ChangeDescription(string)
  | ChangeVendor(Vendor.t)
  | ChangeExpenseType(ExpenseType.t)
  | ChangeDate(Date.t)
  | AddSubTotal
  | RemoveSubTotal(Expense.SubTotal.t)
  | ChangeTax(Money.t)
  | ChangeTotal(Money.t)
  | ChangeNewSubtotalAmount(Money.t)
  | ChangeNewSubtotalTaxRate(Percent.t)
  | Validate(string, bool);

let newSubtotal = state : Expense.SubTotal.t => {
  amount: state.newSubtotalAmount,
  taxRate: state.newSubtotalTaxRate,
};

let add = (item, list) => List.concat([list, [item]]);

let component = ReasonReact.reducerComponent("ExpenseManagementEditor");

let make =
    (
      ~expense=?,
      ~allVendors,
      ~allExpenseTypes,
      ~onCreate=(_) => (),
      ~onModify=(_) => (),
      ~onRemove=(_) => (),
      ~onCancel,
      _children,
    ) => {
  ...component,
  initialState: () =>
    switch (expense) {
    | None => {
        description: "",
        vendor: {
          id: "",
          name: "None",
        },
        expenseType: {
          id: "",
          name: "None",
        },
        date: ConfigurableDate.now(),
        subTotals: [],
        tax: 0,
        total: 0,
        newSubtotalAmount: 0,
        newSubtotalTaxRate: 0,
        isValid: true,
        fields: fieldsAllInvalid,
      }
    | Some(e) => {
        description: e.description,
        vendor: e.vendor,
        expenseType: e.expenseType,
        date: e.date,
        subTotals: e.subTotals,
        tax: e.tax,
        total: e.total,
        newSubtotalAmount: 0,
        newSubtotalTaxRate: 0,
        isValid: true,
        fields: fieldsAllValid,
      }
    },
  reducer: (action, state) =>
    switch (action) {
    | ChangeDescription(newVal) =>
      ReasonReact.Update({...state, description: newVal})
    | ChangeVendor(vendor) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, vendor},
        (self => self.send(Validate("Vendor", true))),
      )
    | ChangeExpenseType(expenseType) =>
      ReasonReact.SilentUpdateWithSideEffects(
        {...state, expenseType},
        (self => self.send(Validate("ExpenseType", true))),
      )
    | ChangeDate(newVal) => ReasonReact.Update({...state, date: newVal})
    | AddSubTotal =>
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          newSubtotalTaxRate: 0,
          newSubtotalAmount: 0,
          subTotals: state.subTotals |> add(newSubtotal(state)),
        },
        (
          self =>
            self.send(
              Validate("SubTotals", self.state.subTotals |> List.length > 0),
            )
        ),
      )
    | RemoveSubTotal(newVal) =>
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          subTotals:
            List.filter(
              (s: Expense.SubTotal.t) =>
                s.amount !== newVal.amount && s.taxRate !== newVal.taxRate,
              state.subTotals,
            ),
        },
        (
          self =>
            self.send(
              Validate("SubTotals", self.state.subTotals |> List.length > 0),
            )
        ),
      )
    | ChangeTax(newVal) => ReasonReact.Update({...state, tax: newVal})
    | ChangeTotal(newVal) => ReasonReact.Update({...state, total: newVal})
    | ChangeNewSubtotalAmount(newVal) =>
      ReasonReact.Update({...state, newSubtotalAmount: newVal})
    | ChangeNewSubtotalTaxRate(newVal) =>
      ReasonReact.Update({...state, newSubtotalTaxRate: newVal})
    | Validate(field, isValid) =>
      let newFields = state.fields;
      Js.Dict.set(newFields, field, isValid);
      ReasonReact.Update({...state, fields: newFields});
    },
  render: self => {
    let totalsCalculateCorrectly = () => {
      let subTotals =
        Belt.List.reduce(self.state.subTotals, 0, (acc, sub) =>
          acc + sub.amount
        );
      let sum = subTotals + self.state.tax;
      sum === self.state.total;
    };
    let getUpdatedSubTotals = state =>
      if (state.newSubtotalAmount > 0 && state.newSubtotalTaxRate > 0) {
        state.subTotals |> add(newSubtotal(state));
      } else {
        state.subTotals;
      };
    let createNewExpense = () => {
      let newExpense: Expense.New.t = {
        description: self.state.description,
        vendor: self.state.vendor,
        expenseType: self.state.expenseType,
        date: self.state.date,
        subTotals: getUpdatedSubTotals(self.state),
        tax: self.state.tax,
        total: self.state.total,
      };
      onCreate(newExpense);
    };
    let modifyExistingExpense = (e: Expense.t) => {
      let modified: Expense.t = {
        id: e.id,
        description: self.state.description,
        vendor: self.state.vendor,
        expenseType: self.state.expenseType,
        date: self.state.date,
        subTotals: getUpdatedSubTotals(self.state),
        tax: self.state.tax,
        total: self.state.total,
      };
      onModify(modified);
    };
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <div className="modify-expense">
      <div className="section">
        <div className="title">
          (s("expense.vendor" |> Lang.translate))
          <ValidationMessage
            hidden=(isValid("Vendor", self.state.vendor.name))
          />
        </div>
        (
          allVendors
          |> List.map((v: Vendor.t) => {
               let className =
                 v.id === self.state.vendor.id ? "card card-selected" : "card";
               <div className onClick=((_) => self.send(ChangeVendor(v)))>
                 (s(v.name))
               </div>;
             })
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
      <div className="section">
        <div className="title">
          (s("expense.type" |> Lang.translate))
          <ValidationMessage
            hidden=(isValid("ExpenseType", self.state.expenseType.name))
          />
        </div>
        (
          allExpenseTypes
          |> List.map((e: ExpenseType.t) => {
               let className =
                 e.id === self.state.expenseType.id ?
                   "card card-selected" : "card";
               <div
                 className onClick=((_) => self.send(ChangeExpenseType(e)))>
                 (s(e.name))
               </div>;
             })
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
      <div className="section">
        <div className="title">
          (s("expense.description" |> Lang.translate))
          <ValidationMessage
            hidden=(isValid("Description", self.state.description))
          />
        </div>
        <textarea
          className="expense-description"
          value=self.state.description
          onChange=(
            ev => {
              self.send(ChangeDescription(getVal(ev)));
              self.send(Validate("Description", getVal(ev) !== ""));
            }
          )
        />
      </div>
      <div className="section">
        <div className="title">
          (s("expense.date" |> Lang.translate))
          <ValidationMessage
            hidden=(isValid("Date", self.state.date |> Date.toDisplay))
          />
        </div>
        <input
          className="date"
          value=(self.state.date |> Date.toDisplay)
          onChange=(ev => self.send(ChangeDate(getVal(ev))))
        />
      </div>
      <div className="section">
        <div className="title">
          (s("expense.subTotals" |> Lang.translate))
          <ValidationMessage
            hidden=(
              isValid(
                "SubTotals",
                self.state.subTotals |> List.length |> string_of_int,
              )
            )
          />
        </div>
        <table>
          <tbody>
            (
              self.state.subTotals
              |> List.map((sub: Expense.SubTotal.t) =>
                   <tr className="subtotal">
                     <td className="amount">
                       (s(sub.amount |> Money.toDisplay))
                     </td>
                     <td className="taxable">
                       (s(sub.taxRate |> Percent.toDisplay))
                     </td>
                     <td>
                       <Button
                         local=true
                         onClick=((_) => self.send(RemoveSubTotal(sub)))
                         label="action.delete"
                       />
                     </td>
                   </tr>
                 )
              |> Array.of_list
              |> ReasonReact.arrayToElement
            )
          </tbody>
          <tfoot>
            <tr className="new-subtotal">
              <td>
                <MoneyInput
                  amount=self.state.newSubtotalAmount
                  key=(self.state.subTotals |> List.length |> string_of_int)
                  onChange=(
                    amount => self.send(ChangeNewSubtotalAmount(amount))
                  )
                />
              </td>
              <td>
                <PercentInput
                  percent=self.state.newSubtotalTaxRate
                  key=(self.state.subTotals |> List.length |> string_of_int)
                  onChange=(
                    percent => self.send(ChangeNewSubtotalTaxRate(percent))
                  )
                />
              </td>
              <td>
                <Button
                  local=true
                  onClick=((_) => self.send(AddSubTotal))
                  label="expense.subTotal.add"
                />
              </td>
            </tr>
          </tfoot>
        </table>
      </div>
      <div className="section">
        <div className="title"> (s("expense.tax" |> Lang.translate)) </div>
        <MoneyInput
          amount=self.state.tax
          onChange=(amount => self.send(ChangeTax(amount)))
        />
      </div>
      <div className="section">
        <div className="title">
          (s("expense.total" |> Lang.translate))
          <ValidationMessage
            hidden=(isValid("Total", self.state.total |> string_of_int))
          />
          <ValidationMessage
            hidden=(totalsCalculateCorrectly())
            messageKey="validation.totalsDoNotAddUp"
          />
        </div>
        <MoneyInput
          amount=self.state.total
          onValidate=(
            v =>
              self.send(
                Validate("Total", v > 0 && ! totalsCalculateCorrectly()),
              )
          )
          onChange=(amount => self.send(ChangeTotal(amount)))
        />
      </div>
      (
        switch (expense) {
        | None =>
          <Button
            local=true
            onClick=((_) => createNewExpense())
            label="action.create"
          />
        /* disabled=(isExpenseValid(self.state.fields) ? false : true) */
        | Some(e) =>
          <span>
            <Button
              local=true
              onClick=((_) => modifyExistingExpense(e))
              label="action.save"
            />
            <Button
              local=true
              className="danger-card"
              onClick=((_) => onRemove(e))
              label="action.delete"
            />
          </span>
        }
      )
      <Button local=true onClick=((_) => onCancel()) label="action.cancel" />
    </div>;
  },
};