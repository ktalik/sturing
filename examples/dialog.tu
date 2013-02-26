Jump:
Tell	_ > Jump
tell	_ > Jump
me	_ > Jump
if	_ > Jump
If	_ > Jump
a	_ > Jump
apple	_ > Fruit
banana	_ > Fruit
orange	_ > Fruit
potato	_ > Vegatable
tomato	_ > Vegetable
pumpkin	_ > Vegetable
_	_ = NoQuestion

Fruit:
is	_ > Fruit
a	_ > Fruit
fruit?	_ > Yes
_	_ > NoQuestion

Vegetable:
is	_ > Vegetable
a	_ > Vegatable
fruit?	_ > No
_	_ > NoQuestion

Yes:
_ YES = ^

No:
_ NO = ^

NoQuestion:
_ You_haven't_given_any_question! = ^
